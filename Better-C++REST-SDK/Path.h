#pragma once

#include <string>
#include <cpprest/http_client.h>
#include <functional>
#include <vector>
#include <experimental/any>

//#include "HandlerFunc.h"

#define HandlerFunc std::function<void(web::http::http_request, std::vector<std::experimental::any>)>
#define RefFunc std::function<int(http_request, vector<any>&, any)>

using namespace std;
using namespace std::experimental;
using namespace web::http;

class Listener;

class Path {
public:
	Path(string path, Listener& listener);

	string toString() const;
	const vector<string> getNodes() const;
	int existsRef(string var);

	template<typename value_T>
	Path& addRef(string var, vector<value_T> list) {
		vector<any> ref;
		for (auto& v : list) ref.push_back(v);
		listRefs[var] = ref;

		return *this;
	}

	template<typename value_T>
	Path& addRef(string var, map<string, value_T> map) {
		std::map<string, any> ref;
		for (auto& v : map) ref[v.first] = v.second;
		mapRefs[var] = ref;

		return *this;
	}

	Path& addRef(string var, RefFunc func);
	
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
	void trigger(method m, vector<any> args, http_request req) {
		vector<HandlerFunc>* funcs;
		if (m == methods::GET) funcs = &gethandlers;
		else if (m == methods::POST) funcs = &posthandlers;
		else if (m == methods::PUT) funcs = &puthandlers;
		else if (m == methods::DEL) funcs = &delhandlers;
		for (auto& func : *funcs) {
			func(req, args);
		}
	}

	int checkNode(http_request req, string node, string node_v, vector<any>&);

private:
	string path;
	Listener& listener;
	vector<string> nodes;
	map<string, RefFunc> refs;
	map<string, vector<any>> listRefs;
	map<string, map<string, any>> mapRefs;
	vector<HandlerFunc> gethandlers;
	vector<HandlerFunc> posthandlers;
	vector<HandlerFunc> puthandlers;
	vector<HandlerFunc> delhandlers;
};