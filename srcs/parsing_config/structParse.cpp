#include "../../includes/Config.hpp"
#include "../../includes/colors.hpp"
#include <sstream>
#include <string>
#include <exception>
#include <dirent.h>

/**
 * @brief verify if there's only one semicolon at the end and remove it
 * 
 * @param str string with value
 * @return std::string string value without the semicolon
 */
static std::string	removeSemicolon(std::string str)
{
	unsigned int		indexSemicolon;
	std::string			result;

	indexSemicolon = str.rfind(';');
	if (indexSemicolon == std::string::npos || indexSemicolon != str.size() - 1)
		throw std::invalid_argument(RED "Invalid argument : semi colon" RESET);
	result = str.substr(0, str.size() - 1);
	return (result);
}
/**
 * @brief Get the Value object
 * 
 * @param data string with the name
 * @return std::string value
 */
static std::vector<std::string>	getValue(std::string data)
{
	std::string					temp;
	std::stringstream			ss(data);
	std::vector<std::string>	infos;

	for (int i = 0; ss >> temp; i++)
		infos.push_back(temp);
	return (infos);
}

/**
 * @brief parsing for the addres and the port
 * 
 * @param data strings of the address:port
 * @return vecAddPort vect of pair address and port
 */
static vecAddPort	parseAddressPort(std::vector<std::string> data)
{
	vecAddPort					result;
	std::string					temp;
	int							size;
	unsigned int				colon;
	unsigned int				port;
	std::vector<std::string>	infos;

	size = data.size();
	for (int i = 0; i < size; i++)
	{
		std::stringstream	ss;
		infos = getValue(data[i]);
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Invalid argument : address port" RESET);
		temp = removeSemicolon(infos[1]);
		colon = temp.find(':');
		if (colon == std::string::npos || colon != temp.rfind(':'))
			throw std::invalid_argument(RED "Invalid argument : address port 1" RESET);
		ss << temp.substr(colon + 1, temp.size() - colon - 1);
		ss >> port;
		if (port > 65535)
			throw std::invalid_argument(RED "Invalid argument : name server" RESET);
		result.push_back(std::make_pair(temp.substr(0, colon), port));
	}
	return (result);
}

/**
 * @brief parsing fct for serverData struct
 * 
 * @param data vect of diff server / address:port
 * @return std::vector<serverData> vect of name and address/port pair
 */
std::vector<serverData>	parseServer(std::vector<hostname> data)
{
	serverData					tempData;
	std::vector<std::string>	infos;
	std::vector<serverData>		result;
	int							nbServer;

	nbServer = data.size();
	for (int i = 0; i < nbServer; i++)
	{
		infos = getValue(data[i].serverName);
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Invalid argument : name server" RESET);
		tempData.name = removeSemicolon(infos[1]);
		tempData.addressPort = parseAddressPort(data[i].addressPort);
		result.push_back(tempData);
	}
	return (result);
}

std::vector<unsigned int>	getCode(std::vector<std::string> data)
{
	unsigned int				sizeData;
	unsigned int				temp;
	std::vector<unsigned int>	result;

	sizeData = data.size();
	for (unsigned int i = 1; i < sizeData - 1; i++)
	{
		std::stringstream			ss;
		ss << data[i];
		ss >> temp;
		if (temp < 100 || temp > 599)
			throw std::invalid_argument(RED "Invalid argument : name server" RESET);
		result.push_back(temp);
	}
	return (result);
}

std::vector<errorData>	parseError(std::vector<std::string> data)
{
	errorData					tempData;
	std::vector<errorData>		result;
	std::string					pathError;
	std::vector<std::string>	infos;
	unsigned int				nbError;
	DIR							dir;

	nbError = data.size();
	for (unsigned int i = 0; i < nbError; i++)
	{
		infos = getValue(data[i]);
		if (infos.size() < 3)
			throw std::invalid_argument(RED "Invalid argument : error path" RESET);
		tempData.path = removeSemicolon(infos[infos.size() - 1]);
		dir = opendir(tempData.path.c_str);
		if (!dir)
			throw std::invalid_argument(RED "Invalid argument : error path" RESET);
		closedir(dir);
		tempData.code = getCode(infos);
		result.push_back(tempData);
	}
	return (result);
}

unsigned int	parseMaxSize(std::string data)
{
	std::vector<std::string>	infos;
	std::stringstream			ss;
	unsigned int				result;


	infos = getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Invalid argument : max size" RESET);
	ss << removeSemicolon(infos[1]);
	ss >> result;
	return (result);
}
