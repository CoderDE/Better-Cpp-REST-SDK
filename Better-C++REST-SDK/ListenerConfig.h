#pragma once

#include <functional>
#include <experimental/any>
#include <cpprest/http_listener.h>

using namespace std;
using namespace std::experimental;
using namespace web;
using namespace web::http;
using namespace web::http::experimental;
using namespace web::http::experimental::listener;

struct ListenerConfig {
	http_listener_config nconfig;

	std::function<void(http_request)> validButFailed;
	std::function<void(http_request)> foundInvalidVar;
	std::function<void(http_request)> notFound;
	std::function<void(http_request, vector<any>&)> before;
	std::function<void(http_request, vector<any>&, int)> after;
};
