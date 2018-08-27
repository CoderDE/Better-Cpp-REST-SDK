#include "Listener.h"

using namespace std;
using namespace std::experimental;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

Listener::Listener(string uri_s, ListenerConfig config) : config(config) {
	web::uri_builder uri(uri_s);
	listener = http_listener(uri.to_uri().to_string(), config.nconfig);
	auto hdl = config.startHandler;
	if (!hdl) hdl = [=](auto req) {
		try {
			vector<any> vars;
			this->handle(req, vars);
		} catch (ListenerException ex) {
			switch (ex.getType()) {
			case ListenerException::NotFound:
				req.reply(status_codes::NotFound);
				break;
			case ListenerException::FoundInvalidVar:
				req.reply(status_codes::BadRequest);
				break;
			case ListenerException::ValidButFailed:
				req.reply(status_codes::BadRequest);
				break;
			default:
				throw ex;
			}
		}
	};
	listener.support(methods::GET, hdl);
	listener.support(methods::POST, hdl);
	listener.support(methods::PUT, hdl);
	listener.support(methods::DEL, hdl);
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

void Listener::handle(http_request req, vector<any>& vars_r) {
	vector<string> nodes = web::uri::split_path(req.relative_uri().path());
	bool foundInvalidVar = false;
	bool validButFailed = false;

	for (Path& path : paths) {
		bool next = false;
		auto n = path.getNodes();
		if (n.size() != nodes.size()) continue;
		vector<any> vars(vars_r);
		for (int i = 0; i < nodes.size(); i++) {
			int err = path.checkNode(req, n.at(i), nodes.at(i), vars);
			if (err != 0) {
				next = true;
				switch (err) {
				case 1:
					break;
				case 2:
					foundInvalidVar = true;
					break;
				case 3:
					validButFailed = true;
					break;
				case 4:
					return;
				default:
					throw ListenerException(ListenerException::Custom, err);
				}
				break;
			}
		}
		if (next) continue;
		path.trigger(req.method(), vars, req);
		return;
	}

	if (validButFailed) {
		throw ListenerException(ListenerException::ValidButFailed);
	}
	else if (foundInvalidVar) {
		throw ListenerException(ListenerException::FoundInvalidVar);
	}
	else {
		throw ListenerException(ListenerException::NotFound);
	}
}

void Listener::run() {
	listener.open().wait();
	while (true) {
		sleep(1);
	}
}