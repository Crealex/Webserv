#include "../includes/Logger.hpp"

// CONSTRUCTOR && DESTRUCTOR
Logger::Logger(t_typePrint type, std::string message)
{
	switch (type)
	{
		case 0 :	printSocketListened(message);
					break;
		case 1 :	printRequest(message);
					break;
		case 2 :	printResponse(message);
					break;
		case 3 :	printError(message);
					break;
	}
}