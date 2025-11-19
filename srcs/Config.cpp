
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

// GETTER FROM addressPort

std::vector<struct pair> Config::getAddressPort() const
{
	return (this->addressPort);
}

// GETTER

std::string Config::getErrorPath() const
{
	return (this->errorPath);
}

unsigned int Config::getMaxSize() const
{
	return (this->maxSize);
}

// GETTER FROM struct site

std::map<std::string, bool>	Config::getMethod(std::string siteName) const
{
	return (this->sites.at(siteName).method); //throw
}

std::vector<std::string>	Config::getRedirection(std::string siteName) const
{
	return (this->sites.at(siteName).redirection);
}

std::string	Config::getDirRoot(std::string siteName) const
{
	return (this->sites.at(siteName).dirRoot);
}

bool	Config::getDirListing(std::string siteName) const
{
	return (this->sites.at(siteName).dirListing);
}

std::string	Config::getDefaultFile(std::string siteName) const
{
	return (this->sites.at(siteName).defaultFile);
}

bool	Config::getUploadFiles(std::string siteName) const
{
	return (this->sites.at(siteName).uploadFiles);
}

std::string	Config::getCGI(std::string siteName) const
{
	return (this->sites.at(siteName).CGI);
}
