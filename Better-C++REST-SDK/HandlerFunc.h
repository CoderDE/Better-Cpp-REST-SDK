#pragma once

#include <memory>
#include <functional>

#define C(...) __VA_ARGS__
#define HF(a, ...) (std::function<void(__VA_ARGS__)>)[ a ](__VA_ARGS__)

class HandlerFunc {
private:
	struct function_b {
		virtual ~function_b() {};
	};

	template<typename... Args>
	struct func : function_b {
		typedef std::function<void(Args...)> func_t;
		func_t funct;
		func(func_t f) : funct(f) {};
	};

	std::unique_ptr<function_b> fb;
public:
	template <typename... Args>
	HandlerFunc(std::function<void(Args...)> f) : fb(new func<Args...>(f)) { };

	template <typename... Args>
	HandlerFunc& operator=(std::function<void(Args...)> f) {
		fb = std::unique_ptr<function_b>(new func<Args...>(f));
		return *this;
		
	};

	template <typename... Args>
	void operator()(Args... args) const {
		func<Args...>* f = dynamic_cast<func<Args...>*>(fb.get());
		if (f)
			f->funct.target<void(*func)(...)>()(args...);
		else
			throw std::runtime_error("Invalid arguments to function object call!");
	};
};