
#include "../includes/Config.hpp"
#include "../includes/colors.hpp"
#include <exception>
#include <stdexcept>

Config::Config(std::string path)
{
	// parsing config file
	std::cout << GREEN << "Default Config constructor called" << RESET << std::endl;
}

Config::~Config()
{
	std::cout << RED << "Destructor called" << RESET << std::endl;
}

// GETTER

std::string Config::getAddress() const
{
	return (this->address);
}

unsigned int Config::getPort() const
{
	return (this->port);
}

std::string Config::getErrorPage() const
{
	return (this->errorPage);
}

unsigned int Config::getMaxSize() const
{
	return (this->maxSize);
}

// GETTER struct file

std::string Config::getProtocol(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).protocol);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "Invalid element attribut" << RESET << std::endl;
		return (NULL);
	}
}

std::string Config::getRedirection(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).redirection);
	} 
	catch (std::exception &e) 
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (NULL);
	}
}

std::string Config::getDirRoot(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).dirRoot);
	} 
	catch (std::exception &e)
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (NULL);
	}
}

bool Config::getDirListing(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).dirListing);
	} 
	catch (std::exception &e)
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (0);
	}

}

std::string Config::getDefaultFile(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).defaultFile);
	} 
	catch (std::exception &e)
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (NULL);
	}
}

bool Config::getUploadFiles(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).uploadFiles);
	} 
	catch (std::exception &e)
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (0);
	}
}

std::string Config::getCGI(std::string siteName) const
{
	try 
	{
		return (this->sites.at(siteName).CGI);
	} 
	catch (std::exception &e)
	{
		std::cerr << "Error, invalid element attribut" << RESET << std::endl;
		return (NULL);
	}
}
