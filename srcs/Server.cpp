#include "../includes/includes.hpp"
#include "../includes/configStruct.hpp"
#include "../includes/Server.hpp"
#include "../includes/printDebug.hpp"
#include <exception>

// Constructor & Destructor

Server::Server(std::string pathServer)
{
	server data;
	(void)pathServer;

	try 
	{
		std::cout << "in create" << std::endl;
		// data = createStruct(pathServer);
		// printConfigStruct(data);
		parseElt(data);
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

void	Server::parseElt(server data)
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
	this->_addressPort = parseAddressPort(data.listen);
	this->_errorPage = parseErrorPage(data.errorPages);
	this->_maxSize = parseMaxSize(data.maxSize);
	printAtt();
}

void const	Server::printAtt() const
{
	std::cout << "Hostname : " << this->_hostname << std::endl;
	int	sizeAddPort = this->_addressPort.size();
	for (int i = 0; i < sizeAddPort; i++)
		std::cout << "listen : " << this->_addressPort[i].first << ":" << this->_addressPort.second << std::endl;
	std::cout << "Root : " << this->_root << std::endl;
	std::cout << "Max size : " << this->_maxSize << std::endl;
	int	sizeErrorPage = this->_errorPage.size();
	for (int i = 0; i < sizeErrorPage; i++)
	{
		std::cout << "Error page : " << std::endl;
		std::cout << "    " << this->_errorPage[i].first << " : " << this->_errorPage[i].second << std::endl;
	}
}