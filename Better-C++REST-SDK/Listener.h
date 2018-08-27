#pragma once

#include <string>
#include <cpprest/http_listener.h>

#include "Path.h"
#include "ListenerConfig.h"
#include "ListenerException.h"

class Listener {
public:
	Listener(std::string uri, ListenerConfig config = ListenerConfig());

	web::http::experimental::listener::http_listener& getNative();
	Path& path(std::string path);
	void run();
	void handle(web::http::http_request req, std::vector<std::experimental::any>& vars_r);

private:
	web::http::experimental::listener::http_listener listener;
	std::vector<Path> paths;
	ListenerConfig config;
};