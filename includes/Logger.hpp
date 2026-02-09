#ifndef LOGGER_HPP

#define LOGGER_HPP

#include "includes.hpp"

typedef	enum	typePrint
{
	socketListened,
	request,
	response,
	error,
}	t_typePrint;

class Logger
{
	public:
		Logger(t_typePrint type, std::string mess);

};

#endif