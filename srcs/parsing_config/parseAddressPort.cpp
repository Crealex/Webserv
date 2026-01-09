#include "../../includes/parseUtils.hpp"

typedef std::pair<std::string, unsigned int> addPort_t;

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
 * @brief parsing for the address and the port
 * 
 * @param data strings of the address:port
 * @return std::vector<addPort_t> vect of pair address and port
 */
std::vector<addPort_t>	parseAddressPort(std::vector<std::string> data)
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
