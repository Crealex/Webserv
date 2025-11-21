#include "../../includes/Config.hpp"
#include "../../includes/colors.hpp"
#include <sstream>
#include <string>
#include <exception>

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
		throw std::invalid_argument(RED "Invalid argument" RESET);
	result = str.substr(0, str.size() - 1);
	return (result);
}
/**
 * @brief Get the Value object
 * 
 * @param data string with the name
 * @return std::string value
 */
static std::string	getValue(std::string data)
{
	std::string					result;
	std::stringstream			ss(data);
	std::vector<std::string>	infos;

	for (int i = 0; ss >> result; i++)
		infos.push_back(result);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Invalid argument" RESET);
	result = removeSemicolon(infos[1]);
	return (result);
}

/**
 * @brief parsing for the addres and the port
 * 
 * @param data strings of the address:port
 * @return vecAddPort vect of pair address and port
 */
static vecAddPort	parseAddressPort(std::vector<std::string> data)
{
	vecAddPort			result;
	std::string			temp;
	int					size;
	unsigned int		colon;
	unsigned int		port;

	size = data.size();
	for (int i = 0; i < size; i++)
	{
		std::stringstream	ss;
		temp = getValue(data[i]);
		colon = temp.find(':');
		if (colon == std::string::npos || colon != temp.rfind(':'))
			throw std::invalid_argument(RED "Invalid argument" RESET);
		ss << temp.substr(colon + 1, temp.size() - colon - 1);
		ss >> port;
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
	std::vector<serverData>		result;
	std::vector<std::string>	temp;
	int							nbServer;

	nbServer = data.size();
	for (int i = 0; i < nbServer; i++)
	{
		tempData.name = getValue(data[i].serverName);
		tempData.addressPort = parseAddressPort(data[i].addressPort);
		result.push_back(tempData);
	}
	return (result);
}

