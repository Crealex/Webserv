#include "../includes/Logger.hpp"

// METHODS

// PRIVATE

void	Logger::_printTime()
{
	std::time_t timeNow;
	struct tm	*timeDisplay;
	char		display[100];

	std::time(&timeNow);
	timeDisplay = std::localtime(&timeNow);
	std::strftime(display, sizeof(display), "Date: %a, %d.%m.%Y - %X", timeDisplay);
	std::cout << display;
}


void	Logger::_printDebug(std::string message)
{
	std::cout << YELLOW;
	std::cout << "[DEBUG] : ";
	_printTime();
	std::cout << RESET;
	std::cout << std::endl;
	std::cout << message;
	std::cout << std::endl;
}

void	Logger::_printInfo(std::string message)
{
	std::cout << CYAN;
	std::cout << "[INFO] : ";
	_printTime();
	std::cout << RESET;
	std::cout << std::endl;
	std::cout << message;
	std::cout << std::endl;
}

void	Logger::_printError(std::string message)
{
	std::cout << RED;
	std::cout << "[ERROR] : ";
	_printTime();
	std::cout << RESET;
	std::cout << std::endl;
	std::cout << message;
	std::cout << std::endl;
}

// PUBLIC

void	Logger::print(type_t type, std::string message, bool isWriting)
{
	if (!isWriting)
		return ;
	switch (type)
	{
		case 0:
			_printDebug(message);
			break;
		
		case 1:
			_printInfo(message);
			break;
		
		case 2:
			_printError(message);
			break;
	}
}