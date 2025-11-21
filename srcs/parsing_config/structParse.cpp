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

std::vector<std::string>	parseServer(structParse data)
{
	std::vector<std::string>	result;
	std::vector<std::string>	temp;
	int							nbHostname;

	nbHostname = data.hostname.size();
	for (int i = 0; i < nbHostname; i++)
	{
		temp = getInfoStruct(data.hostname);
		result[i] = removeSemicolon(temp[1]);
		result[i].compare("missing")
			throw std::invalid_argument(RED "Invalid argument" RESET);
	}
	return (address);
}

std::map<std::string, pair>	parseAdressPort(structParse data)
{
	std::map<std::string, pair>	result;
	std::vector<pair>	temp;
	int							nb;

	nb = data.address.size();
	for (int i = 0; i < nb; i++)
	{
		temp[i].p1 = getInfoStruct(data.address);
		temp[i].p2 = 
		
	}


}