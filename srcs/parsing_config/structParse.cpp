#include "Config.hpp"
#include "structParse.hpp"
#include "colors.hpp"
#include <sstream>
#include <string>
#include <exception>

static std::vector<std::string>	getInfoStruct(std::string data)
{
	std::stringstream			ss;
	std::vector<std::string>	infos;

	ss << data;
	for (int j = 0; getline(ss, infos[i], ' '););
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Invalid argument" RESET);
	return (infos);
}

static std::string	removeSemicolon(std::string str)
{
	int			indexSemicolon;
	std::string	result;

	indexSemicolon = str.rfind(';');
	if (indexSemicolon == std::string::npos || indexSemicolon != str.size() - 1)
		throw std::invalid_argument(RED "Invalid argument" RESET);
	result = str.substr(0, str.size() - 1);
	return (result);
}

std::vector<serverData>	parseServer(std::vector<hostname> data)
{
	std::vector<serverData>		result;
	std::vector<std::string>	temp;
	int							nbServer;

	nbServer = data.size();
	for (int i = 0; i < nbServer; i++)
	{
		temp = getInfoStruct(data[i].serverName);
		result[i] = removeSemicolon(temp[1]);
	}
	return (address);
}
