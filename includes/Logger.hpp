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
		enum type
		{
			DEBUG,
			INFO,
			ERROR,
		};

		static void	log(enum type type, std::string message, bool isWriting);
};

#endif