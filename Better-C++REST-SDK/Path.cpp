#include "Path.h"

#include <functional>

#include "Listener.h"

using namespace std;
using namespace std::experimental;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

Path::Path(string path, Listener& listener) : listener(listener) {
	/*int len = path.length();
	{
		char start = path[0];
		if (start == '/' || start == '\\') path = path.substr(1, len);
	}
	if (len < 1) return;
	this->path = path;

	size_t pos;
	while (pos = path.find('/') != path.npos) {
		nodes.push_back(path.substr(0, pos));
		path.erase(0, pos+1);
	}

	if (path.length() < 1) return;
	nodes.push_back(path);*/
	nodes = web::uri::split_path(path);
}

string Path::toString() const {
	return path;
}

const vector<string> Path::getNodes() const {
	return nodes;
}

Path::RefType Path::getRefType(string var) {
	if (refs.find(var) != refs.end()) return Path::RefType::Func;
	if (listRefs.find(var) != listRefs.end()) return Path::RefType::List;
	if (mapRefs.find(var) != mapRefs.end()) return Path::RefType::Map;
	return Path::RefType::None;
}

Path& Path::addRef(string var, RefFunc func) {
	refs[var] = func;

	return *this;
}

/*
0 = success
1 = not found
2 = invalid var
3 = valid var, but failed
4 = break, already apllied
*/
int Path::checkNode(http_request req, string node_t, string node_v, vector<any>& vars) {
	if (node_t[0] != '{') return node_t != node_v;
	node_t.erase(0, 1);
	size_t pos;
	if ((pos = node_t.find(':')) == node_t.npos) return node_t != node_v;
	string var_n = node_t.substr(0, pos);
	node_t.erase(0, pos + 1);
	if ((pos = node_t.find('}')) == node_t.npos) return node_t != node_v;
	string var_t = node_t.substr(0, pos);
	any v;
	if (var_t == "int") {
		if (!node_v.empty() && std::find_if(node_v.begin(), node_v.end(), [](char c) { return !std::isdigit(c); }) == node_v.end()) {
			v = atoi(node_v.c_str());
		} else {
			return 2;
		}
	} else if (var_t == "str") {
		v = node_v;
	} else {
		return 1;
	}

	switch (getRefType(var_n)) {
	case Path::RefType::Func: {
		return refs[var_n](req, vars, v);
	} case Path::RefType::List: {
		if (var_t != "int") return 2;
		vars.push_back(v);
		try {
			vars.push_back(listRefs[var_n].at(any_cast<int>(v)));
		} catch (out_of_range) {
			return 3;
		}
		return 0;
	} case Path::RefType::Map: {
		if (var_t != "str") {
			if (var_t == "int") v = to_string(any_cast<int>(v));
			else return 2;
		}
		vars.push_back(v);
		try {
			vars.push_back(mapRefs[var_n].at(any_cast<string>(v)));
		} catch (out_of_range) {
			return 3;
		}
		return 0;
	}
	};

	vars.push_back(v);
	return 0;

	/*int inVar = 0;
	size_t var_start = 0;
	size_t var_length = 0;
	string var_n = "";
	string var_t = "";

	int i = 0;
	int j = 0;
	for (char c : node) {
		if (c == '{') {
			if (inVar == 1) {} 
			else if (inVar) {
				var_start = var_start + var_length;
				var_length = 1;
				var_n = "";
				var_t = "";
			} else {
				inVar = 1;
				var_start = i;
				var_length = 1;
			}
		} else if (c == ':') {
			if (inVar == 2) {
				inVar = 0;
				var_length++;
				if (node_v.substr(var_start, var_length) == node.substr(var_start, var_length)) i += var_length;
				else return false;
			} else if (inVar == 1) {
				inVar = 2;
				var_length++;
			} else if (c != node_v[i++]) return false;
		} else if (c == '}') {
			if (inVar == 1) {
				inVar = 0;
				var_length++;
				if (node_v.substr(var_start, var_length) == node.substr(var_start, var_length)) i += var_length;
				else return false;
			} else if (inVar == 2) {
				// this lines arent completly correct
				if (var_t == "str") inVar = 3;
				else inVar = 0;
				// here needs to be something
			}
		} else if (inVar == 1) {
			var_n += c;
			var_length++;
		} else if (inVar == 2) {
			var_t += c;
			var_length++;
		} else if (node_v[i++] != c) return false;
		j++;
	}*/
}