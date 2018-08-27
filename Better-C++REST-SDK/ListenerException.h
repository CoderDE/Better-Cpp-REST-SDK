#pragma once

#include <exception>

class ListenerException : public std::exception
{
public:
	enum Type {
		NotFound,
		FoundInvalidVar,
		ValidButFailed,
		Custom
	};
	
	ListenerException(Type type, int customError = 0);

	Type getType();
	int getCustomError();

private:
	Type type;
	int customError;
};

