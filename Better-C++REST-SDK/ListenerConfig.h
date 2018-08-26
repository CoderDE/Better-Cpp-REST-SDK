#pragma once

#include <functional>
#include <experimental/any>
#include <cpprest/http_listener.h>

struct ListenerConfig {
	web::http::experimental::listener::http_listener_config nconfig;

	std::function<void(web::http::http_request)> validButFailed;
	std::function<void(web::http::http_request)> foundInvalidVar;
	std::function<void(web::http::http_request)> notFound;
	std::function<void(web::http::http_request, std::vector<std::experimental::any>&)> before;
	std::function<void(web::http::http_request, std::vector<std::experimental::any>&, int)> after;
};
