#pragma once

#include <functional>
#include <experimental/any>
#include <cpprest/http_listener.h>

struct ListenerConfig {
	web::http::experimental::listener::http_listener_config nconfig;

	std::function<void(web::http::http_request)> startHandler;
};
