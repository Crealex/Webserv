#ifndef	STRUCT_PARSE_HPP

#define STRUCT_PARSE_HPP

#include "includes.hpp"

struct serverData;
struct errorData;

struct siteParse
{
	std::string	siteName;
	std::string	method;
	std::string	redirection;
	std::string	dirRoot;
	std::string	dirListing;
	std::string	defaultFile;
	std::string	uploadFiles;
	std::string	CGI;
};

struct hostname
{
	std::string					serverName;
	std::vector<std::string>	addressPort;
};

struct structParse
{
	std::vector<hostname>		vServer;
	std::vector<std::string>	ErrorPage;
	std::string					maxSize;
	std::vector<siteParse>		site;
};



std::vector<serverData>	parseServer(std::vector<hostname> data);
std::vector<errorData>	parseError(std::vector<std::string> data);
unsigned int	parseMaxSize(std::string data);
struct structParse createStruct(std::string configPath);

#endif
