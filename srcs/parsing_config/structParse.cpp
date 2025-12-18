#include "../../includes/Config.hpp"
#include "../../includes/structParse.hpp"
#include "../../includes/colors.hpp"
#include <sstream>
#include <fstream>

/**
 * @brief verify if there's only one semicolon at the end and remove it
 * 
 * @param str string with value
 * @return std::string string value without the semicolon
 */
static std::string	removeSemicolon(std::string str)
{
	size_t		indexSemicolon;
	std::string	result;

	indexSemicolon = str.rfind(';');
	if (indexSemicolon == std::string::npos || indexSemicolon != str.size() - 1)
		throw std::invalid_argument(RED "Error : missing or multiple semicolon" RESET);
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

	while (ss >> temp)
		infos.push_back(temp);
	return (infos);
}

/**
 * @brief check if the value is not higher than UNIT_MAX and lower than 0
 * 
 * @param str string with the value (number)
 * @param isMaxSize boolean to know if it is used for maxSize of for another value
 * @return unsigned int return the value (number) 
 */
static unsigned int	checkDigitValue(std::string str, bool isMaxSize)
{
	unsigned int		res;
	size_t				firstDigitNotZero;
	std::string			allDigitsNotZero;
	std::string			temp;
	std::stringstream	ss;

	allDigitsNotZero = "123456789";
	if (str.find('-') != std::string::npos)
		throw std::invalid_argument(RED "Error : invalid digital value, value is negative" RESET);
	firstDigitNotZero = str.find_first_of(allDigitsNotZero.c_str(), 0);
	if (firstDigitNotZero == std::string::npos)
		return (0);
	else if (firstDigitNotZero > 0)
		temp = str.substr(firstDigitNotZero, str.size() - firstDigitNotZero);
	else
		temp = str;
	if (temp.size() > 10 || (temp.size() == 10 && temp.compare("4294967295") > 0))
	{
		temp = "4294967295";
		if (isMaxSize)
			std::cout << GREEN << "Max size value too high : max size set to 4294967295" << RESET << std::endl;
	}
	ss << temp;
	ss >> res;
	return (res);
}

/**
 * @brief return the IP address matching with the keyword
 * 
 * @param word 
 * @return std::string 
 */
static std::string	keywordAddress(std::string word)
{
	if (word.compare("default") == 0)
		return ("0.0.0.0"); 
	else if (word.compare("localhost") == 0)
		return ("127.0.0.1");
	else
		throw std::invalid_argument(RED "Error : invalid IP address, not a valid keyword" RESET);
}

/**
 * @brief check if it's an private IP address
 * 
 * @param infos contains four values of IP address
 * @return true is private
 * @return false isn't private
 */
static bool	isPrivate(std::vector<std::string> infos)
{
	if (infos[0].size() == 2 && infos[0].compare("10") == 0)
		return (true);
	else if (infos[0].size() == 3 && infos[0].compare("172") == 0)
	{
		if (infos[1].size() == 2 && (infos[0].compare("31") <= 0 && infos[0].compare("16") > 0))
			return (true);
	}
	else if (infos[0].size() == 3 && infos[0].compare("192") == 0 && infos[1].size() == 3 && infos[1].compare("168") == 0)
		return (true);
	return (false);
}

static bool	isValidAddress(std::vector<std::string> infos)
{
	size_t	nbInfos;
	size_t	sizeElt;

	nbInfos = infos.size();
	for (size_t i = 0; i < nbInfos; i++)
	{
		sizeElt = infos[i].size();
		if (sizeElt > 3 || (sizeElt == 3 && infos[i].compare("255") > 0))
			return (false);
	}
	return (true);
}

/**
 * @brief check the IP address
 * 
 * @param address string with IP address
 * @param colon position of the colon
 * @return std::string right address
 */
static std::string	checkAddress(std::string addressPort, size_t colon)
{
	std::string					result;
	std::string					temp;
	std::string					address;
	std::stringstream			ss;
	std::vector<std::string>	infos;
	size_t						size;
	
	if (colon == 0)
		result = "127.0.0.1";
	else
	{
		address = addressPort.substr(0, colon);
		ss << address;
		for (;getline(ss, temp, '.');)
			infos.push_back(temp);
		size = infos.size();
		switch (size)
		{
			case 1:
				result = keywordAddress(infos[0]);
				break;
			case 4:
				if (isPrivate(infos) || !isValidAddress(infos))
					throw std::invalid_argument(RED "Error : invalid IP address, IP address is either private or doesn't exist" RESET);
				result = address;
				break;
			default:
				throw std::invalid_argument(RED "Error : invalid IP address" RESET);
		}
	}
	return (result);
}

/**
 * @brief parsing for the addres and the port
 * 
 * @param data strings of the address:port
 * @return std::vector<addPort_t> vect of pair address and port
 */
static std::vector<addPort_t>	parseAddressPort(std::vector<std::string> data)
{
	std::vector<addPort_t>		result;
	std::string					address;
	int							size;
	unsigned int				port;
	size_t						colon;
	std::vector<std::string>	infos;

	size = data.size();
	for (int i = 0; i < size; i++)
	{
		infos = getValue(removeSemicolon(data[i]));
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Error : missing or multiple address / port" RESET);
		colon = infos[1].find(':');
		if (colon == std::string::npos || colon != infos[1].rfind(':'))
			throw std::invalid_argument(RED "Error : missing or multiple colon for address / port" RESET);
		address = checkAddress(infos[1], colon);
		if (colon == infos[1].size() - 1)
			port = 4242;
		else
			port = checkDigitValue(infos[1].substr(colon + 1, infos[1].size() - colon - 1), false);
		if (port > 65535)
			throw std::invalid_argument(RED "Error : invalid port value" RESET);
		result.push_back(std::make_pair(address, port));
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
		infos = getValue(removeSemicolon(data[i].serverName));
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Error : invalid hostname" RESET);
		tempData.name = infos[1];
		tempData.addressPort = parseAddressPort(data[i].addressPort);
		result.push_back(tempData);
	}
	return (result);
}

static void	checkDuplicateCode(std::vector<errorData> globalRes, unsigned int toCheck, std::string actualPath, bool &duplicateOk)
{
	unsigned int	nbCodeInVec;
	unsigned int	sizeData;

	sizeData = globalRes.size();
	for (unsigned int i = 0; i < sizeData; i++)
	{
		nbCodeInVec = globalRes[i].code.size();
		for (unsigned int j = 0; j < nbCodeInVec; j++)
		{
			if (toCheck == globalRes[i].code[j])
			{
				if (globalRes[i].path.compare(actualPath) != 0)
					throw std::invalid_argument(RED "Error : code with multiple error page" RESET);
				else
					duplicateOk = true;
			}
		}
	}
}

std::vector<unsigned int>	getCode(std::vector<std::string> data, std::string actualPath, std::vector<errorData> globalRes)
{
	unsigned int				sizeData;
	unsigned int				temp;
	bool						duplicate;
	std::vector<unsigned int>	result;

	sizeData = data.size();
	for (unsigned int i = 1; i < sizeData - 1; i++)
	{
		duplicate = false;
		temp = checkDigitValue(data[i], false);
		if (temp < 100 || temp > 599)
			throw std::invalid_argument(RED "Error : invalid code error" RESET);
		checkDuplicateCode(globalRes, temp, actualPath, duplicate);
		for (unsigned int i = 0; i < result.size(); i++)
		{
			if (temp == result[i])
				duplicate = true;
		}
		if (!duplicate)
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
	std::fstream				file;

	nbError = data.size();
	for (unsigned int i = 0; i < nbError; i++)
	{
		infos = getValue(removeSemicolon(data[i]));
		if (infos.size() < 3)
			throw std::invalid_argument(RED "Error : missing path or code for error pages" RESET);
		tempData.path = infos[infos.size() - 1];
		file.open(tempData.path.c_str(), std::ios::in);
		if (!file.is_open())
			throw std::invalid_argument(RED "Error : invalid path for error pages" RESET);
		file.close();
		tempData.code = getCode(infos, tempData.path, result);
		result.push_back(tempData);
	}
	return (result);
}

unsigned int	parseMaxSize(std::string data)
{
	std::vector<std::string>	infos;
	std::string					temp;
	unsigned int				result;


	infos = getValue(removeSemicolon(data));
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple max size" RESET);
	result = checkDigitValue(infos[1], true);
	return (result);
}
