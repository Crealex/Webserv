#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/printDebug.hpp"

// Constructor & Destructor

Server::Server()
{
}

Server::Server(struct server data)
{
	printStructV2(data);
	this->_parseElt(data);

	//std::cout << GREEN << "Default Server constructor called" << RESET << std::endl;
}

Server::~Server()
{
	//std::cout << RED << "Destructor called" << RESET << std::endl;
}


// GETTER

std::string const &Server::getHostname() const
{
	return (this->_hostname);
}

std::vector<addPort_t> const &Server::getAddressPort() const
{
	return (this->_addressPort);
}

std::string const	&Server::getRoot() const
{
	return (this->_root);
}

unsigned int const &Server::getMaxSize() const
{
	return (this->_maxSize);
}

std::map<unsigned int, std::string> const	&Server::getErrorPage() const
{
	return (this->_errorPage);
}

std::vector<Location> const	&Server::getLocations() const
{
	return (this->_locations);
}

std::pair<std::string, std::string>	const	&Server::getCgiHandler() const
{
	return (this->_cgiHandler);
}

// METHODS
// PRIVATE
std::vector<std::string>	Server::_getValue(std::string data)
{
	std::string					temp;
	std::stringstream			ss(data);
	std::vector<std::string>	infos;

	while (ss >> temp)
		infos.push_back(temp);
	return (infos);
}

bool	Server::_isValidDigit(std::string str)
{
	size_t		firstNotDigit;
	std::string	allDigits;

	allDigits = "0123456789";
	firstNotDigit = str.find_first_not_of(allDigits.c_str(), 0);
	if (firstNotDigit == std::string::npos)
		return (false);
	return (true);
}

unsigned int	Server::_checkDigitValue(std::string str, bool isMaxSize)
{
	unsigned int		res;
	size_t				firstDigitNotZero;
	std::string			allDigitsNotZero;
	std::string			temp;
	std::stringstream	ss;

	if (this->_isValidDigit(str))
		throw std::invalid_argument(RED "Error : invalid digital value : " + str + RESET);

	allDigitsNotZero = "123456789";
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

std::string	Server::_parseHostname(std::string data)
{
	std::vector<std::string>	infos;

	infos = this->_getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple hostname" RESET);
	return (infos[1]);
}

std::string Server::_parseRoot(std::string data)
{
	std::vector<std::string>    infos;
	int							isAccessible;

	infos = this->_getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple root path" RESET);
	isAccessible = access(infos[1].c_str(), F_OK & R_OK);
	if (isAccessible == -1)
		throw std::invalid_argument(RED "Error : invalid root path" RESET);
	return (infos[1]);
}

std::string	Server::_keywordAddress(std::string word)
{
	if (word.compare("default") == 0)
		return ("0.0.0.0"); 
	else if (word.compare("localhost") == 0)
		return ("127.0.0.1");
	else
		throw std::invalid_argument(RED "Error : invalid IP address, not a valid keyword" RESET);
}

bool	Server::_isPrivate(std::vector<std::string> infos)
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

bool	Server::_isValidAddress(std::vector<std::string> infos)
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

std::string	Server::_checkAddress(std::string addressPort, size_t colon)
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
				result = this->_keywordAddress(infos[0]);
				break;
			case 4:	
				if (this->_isPrivate(infos) || !this->_isValidAddress(infos))
					throw std::invalid_argument(RED "Error : invalid IP address, IP address is either private or doesn't exist" RESET);
				result = address;	
				break;
			default:	
				throw std::invalid_argument(RED "Error : invalid IP address" RESET);
		}		
	}	
	return (result);
}	

bool	Server::_isDuplicate(std::string add, unsigned int port, std::vector<addPort_t> res)
{
	int	sizeRes;

	sizeRes = res.size();
	for (int i = 0; i < sizeRes; i++)
	{
		if (res[i].first == add && res[i].second == port)
			return (true);
	}		
	return (false);
}

std::vector<addPort_t>	Server::_parseAddressPort(std::vector<std::string> data)
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
		infos = this->_getValue(data[i]);
		if (infos.size() != 2)
			throw std::invalid_argument(RED "Error : missing or multiple address / port" RESET);
		colon = infos[1].find(':');
		if (colon == std::string::npos || colon != infos[1].rfind(':'))
			throw std::invalid_argument(RED "Error : missing or multiple colon for address / port" RESET);
		address = this->_checkAddress(infos[1], colon);
		if (colon == infos[1].size() - 1)
			port = 4242;
		else
			port = this->_checkDigitValue(infos[1].substr(colon + 1, infos[1].size() - colon - 1), false);
		if (port > 65535)
			throw std::invalid_argument(RED "Error : invalid port value" RESET);
		if (!this->_isDuplicate(address, port, result))
			result.push_back(std::make_pair(address, port));
	}
	return (result);
}

std::map<unsigned int, std::string>	Server::_parseErrorPage(std::vector<std::string> data)
{
	std::map<unsigned int, std::string>				result;
	std::vector<std::string>						infos;
	unsigned int									code;
	unsigned int									nbError;
	std::map<unsigned int, std::string>::iterator	it;

	nbError = data.size();
	for (unsigned int i = 0; i < nbError; i++)
	{
		infos = this->_getValue(data[i]);
		if (infos.size() != 3)
			throw std::invalid_argument(RED "Error : missing path / code or multiple path / code for error pages" RESET);
		code = this->_checkDigitValue(infos[1], false);
		if (code < 100 || code > 599)
			throw std::invalid_argument(RED "Error : invalid code error" RESET);
		it = result.find(code);
		if (it == result.end())
		{
			result[code] = infos[2];
		}
	}
	return (result);
}

void	Server::_checkMetricPrefix(std::vector<std::string> &infos)
{
	std::string	metricPrefix;
	size_t		firstNotPrefix;

	metricPrefix = "kKmMgGtT";
	firstNotPrefix = infos[2].find_first_not_of(metricPrefix.c_str(), 0);
	if (firstNotPrefix != std::string::npos)
		throw std::invalid_argument(RED "Error : invalid max size, invalid metric prefix" RESET);
	if (infos[2] == "k" || infos[2] == "K")
		infos[1].append("000");
	if (infos[2] == "m" || infos[2] == "M")
		infos[1].append("000000");
	if (infos[2] == "g" || infos[2] == "G")
		infos[1].append("000000000");
}

unsigned int	Server::_parseMaxSize(std::string data)
{
	std::vector<std::string>	infos;
	int							sizeInfos;
	std::string					temp;
	unsigned int				result;

	infos = this->_getValue(data);
	sizeInfos = infos.size();
	if (sizeInfos < 2 || sizeInfos > 3)
		throw std::invalid_argument(RED "Error : invalid max size" RESET);
	if (sizeInfos == 3)
		this->_checkMetricPrefix(infos);
	result = this->_checkDigitValue(infos[1], true);
	return (result);
}

void	Server::_parseElt(struct server data)
{
	this->_hostname = this->_parseHostname(data.hostname);
	this->_root = this->_parseRoot(data.root);
	this->_addressPort = this->_parseAddressPort(data.listen);
	this->_errorPage = this->_parseErrorPage(data.errorPages);
	this->_maxSize = this->_parseMaxSize(data.maxSize);
	this->_locations = createLocations(data);
	this->_printAtt();
}

void Server::_printAtt() const
{
	std::cout << "Hostname : " << this->_hostname << std::endl;
	int	sizeAddPort = this->_addressPort.size();
	for (int i = 0; i < sizeAddPort; i++)
		std::cout << "listen : " << this->_addressPort[i].first << ":" << this->_addressPort[i].second << std::endl;
	std::cout << "Root : " << this->_root << std::endl;
	std::cout << "Max size : " << this->_maxSize << std::endl;
	for (std::map<unsigned int, std::string>::const_iterator it = this->_errorPage.begin(); it != this->_errorPage.end(); it++)
	{
		std::cout << "Error page : " << std::endl;
		std::cout << "    " << it->first << " : " << it->second << std::endl;
	}
}
