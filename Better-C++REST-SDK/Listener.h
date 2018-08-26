#pragma once

#include <string>
#include <cpprest/http_listener.h>

#include "Path.h"
#include "ListenerConfig.h"

using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

class Listener {
public:
	Listener(string uri, ListenerConfig config = ListenerConfig{ http_listener_config(),
		[](auto req) {req.reply(status_codes::NotAcceptable); },
		[](auto req) {req.reply(status_codes::BadRequest); },
		[](auto req) {req.reply(status_codes::NotFound); },
	});

	http_listener& getNative();
	Path& path(string path);
	void run();

private:
	http_listener listener;
	vector<Path> paths;
	ListenerConfig config;

	void handle(http_request req);
};