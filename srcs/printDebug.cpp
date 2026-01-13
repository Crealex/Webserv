
#include <iostream>
#include "../includes/colors.hpp"
#include "../includes/structParse.hpp"
#include "../includes/Config.hpp"
#include "../includes/configStruct.hpp"

//void	printConfigStruct(structParse config)
//{
//	std::cout << GREEN << "The struct:" << RESET << std::endl;
//	std::cout << BOLD << "maxSize: " << RESET << config.maxSize << std::endl;
//	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
//	for (long unsigned int i = 0; i < config.vServer.size(); i++)
//	{
//		std::cout << BOLD << "	hostname " << i << ": " << RESET << config.vServer[i].serverName << std::endl;
//		for (long unsigned int j = 0; j < config.vServer[i].addressPort.size(); j++)
//			std::cout << "		listen: " << config.vServer[i].addressPort[j] << std::endl;
//	}
//	std::cout << BLUE << "errorPage:" RESET << std::endl;
//	for (long unsigned int i = 0; i < config.ErrorPage.size(); i++)
//std::cout << BOLD << "	" << i << RESET << ": " << config.ErrorPage[i] << std::endl;
//	for (long unsigned int i = 0; i < config.site.size(); i++)
//	{
//		std::cout << BLUE << "site " << i << ": " << RESET << std::endl;
//		std::cout << BOLD << "	siteName : " << RESET << config.site[i].siteName << std::endl;
//		std::cout << BOLD << "	methods : " << RESET << config.site[i].method << std::endl;
//		std::cout << BOLD << "	redirection : " << RESET << config.site[i].redirection << std::endl;
//		std::cout << BOLD << "	dirRoot : " << RESET << config.site[i].dirRoot << std::endl;
//		std::cout << BOLD << "	dirListing : " << RESET << config.site[i].dirListing << std::endl;
//		std::cout << BOLD << "	defaultFile : " << RESET << config.site[i].defaultFile << std::endl;
//		std::cout << BOLD << "	uploadFiles : " << RESET << config.site[i].uploadFiles << std::endl;
//		std::cout << BOLD << "	CGI : " << RESET << config.site[i].CGI << std::endl;
//	}
//}
//
//void printConfigClass(Config conf)
//{
//	std::cout << GREEN << "Class config:" << RESET << std::endl;
//	std::cout << BOLD << "MaxSize: " << RESET << conf.getMaxSize() << std::endl;
//	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
//	for (unsigned int i = 0; i < conf.getAddressPort().size(); i++)
//	{
//		std::cout << BOLD << "	hostname " << i << ": " << RESET << conf.getAddressPort()[i].name << std::endl;
//		for (unsigned int j = 0; j < conf.getAddressPort()[i].addressPort.size(); j++) 
//			std::cout << BOLD << "		listen:" << conf.getAddressPort()[i].addressPort[j].first << ":" << conf.getAddressPort()[i].addressPort[j].second << std::endl;
//	}
//	std::cout << BLUE << "ErrorPage:" << RESET << std::endl;
//	for (unsigned int i = 0; i < conf.getErrorPage().size(); i++)
//	{
//		std::cout << BOLD << "	code:" << RESET;
//		for (unsigned int j = 0; j < conf.getErrorPage()[i].code.size(); j++)
//			std::cout << conf.getErrorPage()[i].code[j];
//		std::cout << std::endl;
//		std::cout << BOLD << "	path:" << RESET << conf.getErrorPage()[i].path << std::endl;
//	}
//	for (unsigned int i = 0; i < conf.getSitesName().size(); i++)
//	{
//		std::cout << BLUE << "site " << i << ": " << RESET << std::endl;
//		std::cout << BOLD << "	siteName : " << RESET << conf.getSitesName()[i] << std::endl;
//		std::cout << BOLD << "	methods : " << RESET;
//		if (conf.getMethod(conf.getSitesName()[i]).at("GET"))
//			std::cout << BOLD << "GET " << RESET;
//		if (conf.getMethod(conf.getSitesName()[i]).at("POST"))
//			std::cout << BOLD << "POST " << RESET;
//		if (conf.getMethod(conf.getSitesName()[i]).at("DELETE"))
//			std::cout << BOLD << "DELETE" << RESET;
//		std::cout << std::endl;
//		std::cout << BLUE << "	redirection : " << RESET << std::endl;
//		for (unsigned int j = 0; j < conf.getRedirection(conf.getSitesName()[i]).size(); j++)
//		std::cout << BOLD << "		" << RESET << conf.getRedirection(conf.getSitesName()[i])[j] << std::endl;
//		std::cout << BOLD << "	dirRoot : " << RESET << conf.getDirRoot(conf.getSitesName()[i]) << std::endl;
//		std::cout << BOLD << "	dirListing : " << RESET << conf.getDirListing(conf.getSitesName()[i]) << std::endl;
//		std::cout << BOLD << "	defaultFile : " << RESET << conf.getDefaultFile(conf.getSitesName()[i]) << std::endl;
//		std::cout << BOLD << "	uploadFiles : " << RESET << conf.getUploadFiles(conf.getSitesName()[i]) << std::endl;
//		std::cout << BOLD << "	CGI : " << RESET << conf.getCGI(conf.getSitesName()[i]) << std::endl;
//	}
//}

void printStructV2(server srv)
{
	int i = 0;
	std::cout << BOLD << BLUE << "server: " << std::endl;
	std::cout << BOLD << "	hostname: " << RESET << srv.hostname << std::endl;
	std::cout << BOLD << "	root: " << RESET << srv.root << std::endl;
	std::cout << BOLD << "	maxSize: " << RESET << srv.maxSize << std::endl;
	std::cout << BOLD << GREEN << "	listen: " << RESET << std::endl;
	while (srv.listen.size() > i)
	{
		std::cout << "		"<< i << ": " << srv.listen.at(i) << std::endl;
		i++;
	}
	i = 0;
	std::cout << BOLD << GREEN << "	errorPages: " << RESET << std::endl;
	while (srv.errorPages.size() > i)
	{
		std::cout << "		" << i << ": " << srv.errorPages.at(i) << std::endl;
		i++;
	}
	std::cout << BOLD << GREEN << "	location:" << RESET << std::endl;
	i = 0;
	while (srv.locations.size() > i)
	{
		std::cout << BOLD << "		autoIndex: " << RESET << srv.locations.at(i).autoIndex << std::endl;
		std::cout << BOLD << "		allowedMethods: " << RESET << srv.locations.at(i).allowedMethods << std::endl;
		std::cout << BOLD << "		index: " << RESET << srv.locations.at(i).index<< std::endl;
		std::cout << BOLD << "		return: " << RESET << srv.locations.at(i).ret << std::endl;
		int j = 0;
		std::cout << BOLD << YELLOW << "		cgi:" << RESET << std::endl;
		while (srv.locations.at(i).cgi.size() > j)
		{
			std::cout << "			" << j << ": " << srv.locations.at(i).cgi.at(j) << std::endl;
			j++;
		}
		std::cout << BOLD << "		uploadPath: " << RESET << srv.locations.at(i).uploadPath << std::endl;
		std::cout << BOLD << "		path: " << RESET << srv.locations.at(i).path << std::endl;
		i++;
	}
}
