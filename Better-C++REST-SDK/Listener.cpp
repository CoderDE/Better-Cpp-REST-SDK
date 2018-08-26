#include "Listener.h"

Listener::Listener(string uri_s, ListenerConfig config) : config(config) {
	web::uri_builder uri(uri_s);
	listener = http_listener(uri.to_uri().to_string(), config.nconfig);
	listener.support(methods::GET, std::bind(&Listener::handle, this, std::placeholders::_1));
}

http_listener& Listener::getNative() {
	return listener;
}

Path& Listener::path(string path) {
	int len = path.length();
	{
		char start = path[0];
		if (start == '/' || start == '\\') path = path.substr(1, len);
	}
	if (len < 1) throw std::invalid_argument("path is invalid");

	for (Path& p : paths) {
		if (p.toString() == path) {
			return p;
		}
	}
	paths.push_back(Path(path, *this));
	return paths.at(paths.size() - 1);
}

void Listener::handle(http_request req) {
	vector<string> nodes = web::uri::split_path(req.relative_uri().path());
	bool foundInvalidVar = false;
	bool validButFailed = false;

	for (Path& path : paths) {
		bool next = false;
		auto n = path.getNodes();
		if (n.size() != nodes.size()) continue;
		vector<any> vars;
		for (int i = 0; i < nodes.size(); i++) {
			int err = path.checkNode(req, n.at(i), nodes.at(i), vars);
			if (err != 0) {
				switch (err) {
				case 2:
					foundInvalidVar = true;
					next = true;
					break;
				case 3:
					validButFailed = true;
					next = true;
					break;
				case 4:
					return;
				}
			} else {
				break;
			}
		}
		if (next) continue;
		path.trigger(req.method(), vars, req);
		return;
	}

	if (validButFailed) {
		config.validButFailed(req);
	} else if (foundInvalidVar) {
		config.foundInvalidVar(req);
	} else {
		config.notFound(req);
	}
}

void Listener::run() {
	listener.open().wait();
	while (true) {
		sleep(1);
	}
}