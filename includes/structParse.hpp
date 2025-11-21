#ifndef	STRUCT_PARSE_HPP

#define STRUCT_PARSE_HPP

#include <iostream>
#include <vector>

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
	std::string serverName;
	std::vector<std::string> addressPort;
};

struct structParse
{
	std::vector<hostname> hostname;
	std::string errorCode;
	std::string errorPath;
	std::string maxSize;
	std::vector<struct siteParse> site;
};

#endif
