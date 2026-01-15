#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/printDebug.hpp"
#include "../includes/Location.hpp"
#include <exception>

// Constructor & Destructor

Server::Server(std::string pathServer)
{
	struct server data;

	try 
	{
		std::cout << "in create" << std::endl;
		data = createStruct(pathServer);
		printStructV2(data);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "in createStruct: " << e.what() << RESET << std::endl;
		return ;
	}
	parseElt(data);

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

// std::vector<Location> const	&Server::getLocations() const
// {
// 	return (this->_locations);
// }

std::pair<std::string, std::string>	const	&Server::getCgiHandler() const
{
	return (this->_cgiHandler);
}

void	Server::parseElt(struct server data)
{
	// this->errorPath = parseErrorPath();

	// for (std::vector<siteParse>::iterator it = data.site.begin();
	// 	it != data.site.end();
	// 	it++)
	// {
	// 	siteParsing((*it));
	// }
	this->_hostname = parseHostname(data.hostname);
	this->_root = parseRoot(data.root);
	std::cout << "euhhh" << std::endl;
	this->_addressPort = parseAddressPort(data.listen);
	// this->_errorPage = parseErrorPage(data.errorPages);
	std::cout << "euhhh" << std::endl;
	this->_maxSize = parseMaxSize(data.maxSize);
	this->_locations = createLocations(data);
	printAtt();
}

void Server::printAtt() const
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
