#pragma once

#include <string>
#include <cpprest/http_client.h>
#include <functional>
#include <vector>
#include <experimental/any>

//#include "HandlerFunc.h"

using HandlerFunc = std::function<void(web::http::http_request, std::vector<std::experimental::any>)>;
using RefFunc = std::function<int(web::http::http_request, std::vector<std::experimental::any>&, std::experimental::any)>;

class Listener;

class Path {
public:
	enum RefType {
		Func,
		List,
		Map,
		None
	};

	Path(std::string path, Listener& listener);

	std::string toString() const;
	const std::vector<std::string> getNodes() const;
	RefType getRefType(std::string var);

	template<typename value_T>
	Path& addRef(std::string var, std::vector<value_T> list) {
		std::vector<std::experimental::any> ref;
		for (auto& v : list) ref.push_back(v);
		listRefs[var] = ref;

		return *this;
	}

	template<typename value_T>
	Path& addRef(std::string var, std::map<std::string, value_T> map) {
		std::map<std::string, std::experimental::any> ref;
		for (auto& v : map) ref[v.first] = v.second;
		mapRefs[var] = ref;

		return *this;
	}

	Path& addRef(std::string var, RefFunc func);
	
	//template<typename...Args>
	Path& get(HandlerFunc handler) {
		gethandlers.push_back(handler);

		return *this;
	}
	
	Path& post(HandlerFunc handler) {
		posthandlers.push_back(handler);

		return *this;
	}

	Path& put(HandlerFunc handler) {
		puthandlers.push_back(handler);

		return *this;
	}

	Path& del(HandlerFunc handler) {
		delhandlers.push_back(handler);

		return *this;
	}

	//template<typename... Args>
	void trigger(web::http::method m, std::vector<std::experimental::any> args, web::http::http_request req) {
		std::vector<HandlerFunc>* funcs;
		if (m == web::http::methods::GET) funcs = &gethandlers;
		else if (m == web::http::methods::POST) funcs = &posthandlers;
		else if (m == web::http::methods::PUT) funcs = &puthandlers;
		else if (m == web::http::methods::DEL) funcs = &delhandlers;
		for (auto& func : *funcs) {
			func(req, args);
		}
	}

	int checkNode(web::http::http_request req, std::string node, std::string node_v, std::vector<std::experimental::any>&);

private:
	std::string path;
	Listener& listener;
	std::vector<std::string> nodes;
	std::map<std::string, RefFunc> refs;
	std::map<std::string, std::vector<std::experimental::any>> listRefs;
	std::map<std::string, std::map<std::string, std::experimental::any>> mapRefs;
	std::vector<HandlerFunc> gethandlers;
	std::vector<HandlerFunc> posthandlers;
	std::vector<HandlerFunc> puthandlers;
	std::vector<HandlerFunc> delhandlers;
};