#ifndef LOGGER_HPP

#define LOGGER_HPP

#include "includes.hpp"

class Logger
{
	private:
		static void	_printTime();
		
		static void	_printDebug(std::string message);
		static void	_printInfo(std::string message);
		static void	_printError(std::string message);
		
	public:
		typedef enum type_t
		{
			DEBUG,
			INFO,
			ERROR,
		};

		static void	print(type_t type, std::string message, bool isWriting);
};

#endif