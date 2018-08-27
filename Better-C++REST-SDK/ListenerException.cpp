#include "ListenerException.h"



ListenerException::ListenerException(Type type, int customError) : type(type), customError(customError)
{
	
}

ListenerException::Type ListenerException::getType() {
	return type;
}

int ListenerException::getCustomError() {
	if (customError < 4 && customError > -1) return 0;
	else return customError;
}