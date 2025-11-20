#include "Config.hpp"
#include "structParse.hpp"
#include "colors.hpp"
#include <sstream>
#include <string>
#include <exception>

std::vector<std::string>	getInfoStruct(std::string data)
{
	std::stringstream			ss;
	std::vector<std::string>	infos;

	ss << data;
	for (int i = 0; getline(ss, infos[i], ' '););
	return (infos);
}

std::string	parseServerAddressPort(structParse data)
{
	std::stringstream			value;
	std::vector<std::string>	result;

	value << data;
	for (int i = 0; getline(value, result[i], ' '););
	if (result[i - 1] != ";")
		throw std::invalid_argument(RED "Invalid argument" RESET);
	 
	return (address);
}