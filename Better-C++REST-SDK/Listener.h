#pragma once

#include <string>
#include <cpprest/http_listener.h>

#include "Path.h"
#include "ListenerConfig.h"

class Listener {
public:
	Listener(std::string uri, ListenerConfig config = ListenerConfig{ web::http::experimental::listener::http_listener_config(),
		[](auto req) {req.reply(web::http::status_codes::NotAcceptable); },
		[](auto req) {req.reply(web::http::status_codes::BadRequest); },
		[](auto req) {req.reply(web::http::status_codes::NotFound); },
	});

	web::http::experimental::listener::http_listener& getNative();
	Path& path(std::string path);
	void run();

private:
	web::http::experimental::listener::http_listener listener;
	std::vector<Path> paths;
	ListenerConfig config;

	void handle(web::http::http_request req);
};