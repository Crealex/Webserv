#include "../includes/structParse.hpp"
#include "../includes/Config.hpp"
#include "../includes/colors.hpp"
#include "../includes/printDebug.hpp"
#include <exception>

// Constructor & Destructor

Config::Config(std::string pathConfig)
{
	structParse data;

	try 
	{
		std::cout << "in create" << std::endl;
		data = createStruct(pathConfig);
		printConfigStruct(data);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "in createStruct: " << e.what() << RESET << std::endl;
		return ;
	}
	parseElt(data);

	//std::cout << GREEN << "Default Config constructor called" << RESET << std::endl;
}

Config::~Config()
{
	//std::cout << RED << "Destructor called" << RESET << std::endl;
}



// GETTER

std::vector<serverData> Config::getAddressPort() const
{
	return (this->addressPort);
}

std::vector<errorData>	Config::getErrorPage() const
{
	return (this->errorPage);
}

unsigned int Config::getMaxSize() const
{
	return (this->maxSize);
}

std::vector<std::string>	Config::getSitesName() const
{
	return (this->sitesName);
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

void	Config::parseElt(structParse data)
{
	// this->errorPath = parseErrorPath();

	for (std::vector<siteParse>::iterator it = data.site.begin();
		it != data.site.end();
		it++)
	{
		siteParsing((*it));
	}
	this->addressPort = parseServer(data.vServer);
	this->errorPage = parseError(data.ErrorPage);
	this->maxSize = parseMaxSize(data.maxSize);
}
