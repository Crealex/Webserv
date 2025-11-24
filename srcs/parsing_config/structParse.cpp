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
			throw std::invalid_argument(RED "Invalid argument" RESET);
		temp = removeSemicolon(infos[1]);
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
	std::vector<std::string>	infos;
	std::vector<serverData>		result;
	int							nbServer;

	nbServer = data.size();
	for (int i = 0; i < nbServer; i++)
	{
		infos = getValue(data[i].serverName);
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Invalid argument" RESET);
		tempData.name = removeSemicolon(infos[1]);
		tempData.addressPort = parseAddressPort(data[i].addressPort);
		result.push_back(tempData);
	}
	return (result);
}

std::vector<unsigned int>	getCode(std::vector<std::string> data)
{
	unsigned int				sizeData;
	std::stringstream			ss;
	unsigned int				temp;
	std::vector<unsigned int>	result;

	sizeData = data.size();
	for (unsigned int i = 1; i < sizeData - 1; i++)
	{
		ss << data[i];
		ss >> temp;
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

	nbError = data.size();
	for (unsigned int i = 0; i < nbError; i++)
	{
		infos = getValue(data[i]);
		if (infos.size() < 3)
			throw std::invalid_argument(RED "Invalid argument" RESET);
		tempData.path = removeSemicolon(infos[infos.size() - 1]);
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
		throw std::invalid_argument(RED "Invalid argument" RESET);
	ss << removeSemicolon(infos[1]);
	ss >> result;
	return (result);
}

int main()
{
	hostname	h1;
	h1.serverName = "hiii";
	h1.addressPort.push_back("localhost:8080");
	h1.addressPort.push_back("localhost:4040");
	hostname	h2;
	h2.serverName = "Hello World";
	h2.addressPort.push_back("default:8080");
	h2.addressPort.push_back("default:4040");
	std::vector<hostname> s;
	s.push_back(h1);
	s.push_back(h2);
	std::vector<serverData>	sResult;
	sResult = parseServer(s);
	for (std::vector<serverData>::iterator it = sResult.begin(); it != sResult.end(); it++)
	{
		std::cout << "Server : " << *it.name << std::end;
		for (std::vector<vecAddPort>::iterator it1 = it.addressPort.begin(); it1 != it.addressPort.end(); it1++)
		{
			std::cout << *it.*it1.first << ":" << *it.*it1.second << std::endl;

		}
	}
	return (0);
}

