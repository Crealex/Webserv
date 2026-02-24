#ifndef LOGGER_HPP

#define LOGGER_HPP

#include "includes.hpp"

class Logger
{
	private:
		
		static void	_printDebug(std::string message);
		static void	_printInfo(std::string message);
		static void	_printError(std::string message);
		static bool	_isEnabled;
		
	public:
		enum type
		{
			DEBUG,
			INFO,
			ERROR,
		};

		static void	log(enum type type, std::string message);
		static void	_printTime();
		static bool	getIsEnabled();
		static void	setIsEnabled(bool status);
};

#endif
