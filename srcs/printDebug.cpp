
#include <iostream>
#include "../includes/colors.hpp"
#include "../includes/structParse.hpp"
#include "../includes/Config.hpp"

void	printConfigStruct(structParse config)
{
	std::cout << GREEN << "The struct:" << RESET << std::endl;
	std::cout << BOLD << "maxSize: " << RESET << config.maxSize << std::endl;
	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
	for (long unsigned int i = 0; i < config.vServer.size(); i++)
	{
		std::cout << BOLD << "	hostname " << i << ": " << RESET << config.vServer[i].serverName << std::endl;
		for (long unsigned int j = 0; j < config.vServer[i].addressPort.size(); j++)
			std::cout << "		listen: " << config.vServer[i].addressPort[j] << std::endl;
	}
	std::cout << BLUE << "errorPage:" RESET << std::endl;
	for (long unsigned int i = 0; i < config.ErrorPage.size(); i++)
		std::cout << BOLD << "	" << i << RESET << ": " << config.ErrorPage[i] << std::endl;
	for (long unsigned int i = 0; i < config.site.size(); i++)
	{
		std::cout << BLUE << "site " << i << ": " << RESET << std::endl;
		std::cout << BOLD << "	siteName : " << RESET << config.site[i].siteName << std::endl;
		std::cout << BOLD << "	methods : " << RESET << config.site[i].method << std::endl;
		std::cout << BOLD << "	redirection : " << RESET << config.site[i].redirection << std::endl;
		std::cout << BOLD << "	dirRoot : " << RESET << config.site[i].dirRoot << std::endl;
		std::cout << BOLD << "	dirListing : " << RESET << config.site[i].dirListing << std::endl;
		std::cout << BOLD << "	defaultFile : " << RESET << config.site[i].defaultFile << std::endl;
		std::cout << BOLD << "	uploadFiles : " << RESET << config.site[i].uploadFiles << std::endl;
		std::cout << BOLD << "	CGI : " << RESET << config.site[i].CGI << std::endl;
	}
}

void printConfigClass(Config conf)
{
	std::cout << GREEN << "Class config:" << RESET << std::endl;
	std::cout << BOLD << "MaxSize: " << RESET << conf.getMaxSize() << std::endl;
	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
	for (unsigned int i = 0; i < conf.getAddressPort().size(); i++)
	{

	}
}
