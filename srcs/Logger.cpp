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
static std::string truncateToLines(const std::string& str, int maxLines)
{
	std::string result;
	int lineCount = 0;
	size_t pos = 0;

	while (pos < str.size() && lineCount < maxLines)
	{
		size_t nextNewline = str.find('\n', pos);
		if (nextNewline == std::string::npos)
		{
			result += str.substr(pos);
			break;
		}
		result += str.substr(pos, nextNewline - pos + 1);
		pos = nextNewline + 1;
		lineCount++;
	}
	if (lineCount >= maxLines && pos < str.size())
		result += "[... truncated ...]\n";
	return result;
}

void	Logger::log(enum type type, std::string message)
{
	std::string truncMessage;

	if (!Logger::_isEnabled)
		return ;
	truncMessage = truncateToLines(message, 12);
	switch (type)
	{
		case Logger::DEBUG:
			_printDebug(truncMessage);
			break;
		
		case Logger::INFO:
			_printInfo(truncMessage);
			break;
		
		case Logger::ERROR:
			_printError(truncMessage);
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
