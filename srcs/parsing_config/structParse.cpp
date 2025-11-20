#include "Config.hpp"
#include "structParse.hpp"
#include "colors.hpp"
#include <sstream>
#include <string>
#include <exception>

std::vector<std::string>	getInfoStruct(std::vector<std::string> data)
{
	std::stringstream			ss;
	std::vector<std::string>	infos;
	unsigned int				size;

	size = data.size();
	for (int i = 0; i < size; i++)
	{
		ss << data[i];
		for (int i = 0; getline(ss, infos[i], ' '););
	}
	return (infos);
}

std::vector<std::string>	parseServer(structParse data)
{
	std::vector<std::string>	result;

	getInfoStruct(data.hostname);
	if (result[i - 1] != ";" || )
		throw std::invalid_argument(RED "Invalid argument" RESET);
	
	return (address);
}