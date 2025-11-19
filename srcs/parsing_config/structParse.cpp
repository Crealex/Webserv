#include "Config.hpp"
#include "structParse.hpp"
#include "colors.hpp"
#include <sstream>
#include <string>

std::string	parseAddress(struct structParse data)
{
	int					colonIndex;
	std::stringstream	value;
	std::string			address;

	colonIndex = data.address.find(':');
	if (colonIndex == std::string::npos)
		throw std::invalid_argument(RED "Invalid argument" RESET);

	value << data.address.substr(colonIndex, data.address.size());
	std::getline(value, address, '\"');
	return (address);
}