#include "../includes/includes.hpp"
#include "../includes/structParse.hpp"
#include "../includes/Server.hpp"
#include "../includes/printDebug.hpp"
#include <exception>

// Constructor & Destructor

Server::Server(std::string pathServer)
{
	structParse data;

	try 
	{
		std::cout << "in create" << std::endl;
		data = createStruct(pathServer);
		printConfigStruct(data);
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

std::vector<Location> const	&Server::getLocations() const
{
	return (this->_locations);
}

std::pair<std::string, std::string>	const	&Server::getCgiHandler() const
{
	return (this->_cgiHandler);
}
