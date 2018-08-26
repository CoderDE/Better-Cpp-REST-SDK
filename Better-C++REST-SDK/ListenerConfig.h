#pragma once

#include <functional>
#include <experimental/any>
#include <cpprest/http_listener.h>

struct ListenerConfig {
	web::http::experimental::listener::http_listener_config nconfig = web::http::experimental::listener::http_listener_config();

	std::function<void(web::http::http_request)> validButFailed = [](auto req) {req.reply(web::http::status_codes::NotAcceptable); };
	std::function<void(web::http::http_request)> foundInvalidVar = [](auto req) {req.reply(web::http::status_codes::BadRequest); };
	std::function<void(web::http::http_request)> notFound = [](auto req) {req.reply(web::http::status_codes::NotFound); };
	std::function<void(web::http::http_request, std::vector<std::experimental::any>&)> before = (std::function<void(web::http::http_request, std::vector<std::experimental::any>&)>) [](auto r, auto& v) { return; };
	std::function<void(web::http::http_request, std::vector<std::experimental::any>&, int)> after = [](auto r, auto& v, auto i) {};
};
