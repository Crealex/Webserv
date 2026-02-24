#include "../includes/Logger.hpp"

// METHODS

// PRIVATE

bool Logger::_isEnabled = false;

void	Logger::_printTime()
{
	std::time_t		timeNow;
	struct tm		*timeDisplay;
	char			display[100];

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

void	Logger::log(enum type type, std::string message)
{
	if (!Logger::_isEnabled)
		return ;
	switch (type)
	{
		case Logger::DEBUG:
			_printDebug(message);
			break;
		
		case Logger::INFO:
			_printInfo(message);
			break;
		
		case Logger::ERROR:
			_printError(message);
			break;
	}
}

void	Logger::setIsEnabled(bool status)
{
	Logger::_isEnabled = status;
}
bool	Logger::getIsEnabled()
{
	return (Logger::_isEnabled);
}
