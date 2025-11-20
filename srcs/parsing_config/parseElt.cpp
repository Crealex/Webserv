#include <iostream>
#include <sstream>
#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include "../../includes/Config.hpp"

static void removeSemiColon(std::string &str)
{
	size_t size = str.size();
	if (str[size] == ';')
		str.erase((str.end())--);
	else
	{
		std::string error("Error: no semicolon on line:\n\t");
		throw std::invalid_argument(error + str);
	}
}

static std::string getSiteName(siteParse site)
{
	std::string sName(site.siteName);
	removeSemiColon(sName);
	std::stringstream ss(sName);
	std::string str;
	ss >> str;
	ss >> str;

	// debug
	std::cout << GREEN << "siteName = " << str << RESET << std::endl;

	return str;
}

static std::map<std::string, bool> createMethod(std::string str)
{
	bool GET = false;
	bool POST = false;
	bool DELETE = false;

	std::map<std::string, bool> method;
	method.insert(std::make_pair(std::string("GET"), GET));
	method.insert(std::make_pair(std::string("POST"), POST));
	method.insert(std::make_pair(std::string("DELETE"), DELETE));

	removeSemiColon(str);
	std::stringstream ss(str);
	std::string word;
	ss >> word;
	while (ss >> word)
	{
		try
		{
			method.at(word) = true;
		}
		catch (...)
		{
			std::string error("Error: invalid variable in line:\n\t");
			throw std::invalid_argument(error + str);
		}
	}

	// debug
	std::cout << "Method :\n\t";
	std::cout << GREEN << std::boolalpha << "Get: " << GET << "\nPOST: " << POST << "\nDELETE: " << DELETE << RESET << std::endl;

	return method;
}

static std::vector<std::string> createRedirection(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string line;
	ss >> line;
	ss >> line;

	std::vector<std::string> ret;
	std::stringstream ss2(line);
	while (std::getline(ss2, line, ','))
	{
		ret.push_back(line);
	}

	// debug
	std::cout << "Redirection contain: \n\t";
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); it++)
	{
		std::cout << *it << " - ";
	}
	std::cout << std::endl;

	return ret;
}

static std::string returnSecond(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	// debug
	std::cout << "Second element of " << str << " is\n\t";
	std::cout << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

static bool returnBool(std::string str)
{
	removeSemiColon(str);
	if (str.find("true") != std::string::npos)
		return true;
	else if (str.find("false") != std::string::npos)
		return false;

	std::string error("Error: invalid variable in line:\n\t");
	throw std::invalid_argument(error + str);
}

static site createStructSite(siteParse site)
{
	struct site ret;

	ret.method = createMethod(site.method);
	ret.redirection = createRedirection(site.redirection);
	ret.dirRoot = returnSecond(site.dirRoot);
	ret.dirListing = returnBool(site.dirListing);
	ret.defaultFile = returnSecond(site.defaultFile);
	ret.uploadFiles = returnBool(site.uploadFiles);
	ret.CGI = returnSecond(site.CGI);

	return ret;
}

void Config::siteParsing(siteParse site)
{
	std::string siteName = getSiteName(site);
	struct site siteStruct = createStructSite(site);

	sites.insert(std::make_pair(siteName, siteStruct));
}
