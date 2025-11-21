#include <iostream>
#include <sstream>
#include <fstream>
#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include "../../includes/Config.hpp"

static void removeSemiColon(std::string &str)
{
	std::string::iterator ite = str.end();
	ite--;
	if (*ite == ';')
		str.erase(ite);
	else
	{
		std::string error("Error: no semicolon on line:\n\t");
		throw std::invalid_argument(error + str);
	}
}

static std::string getSiteName(std::string sName)
{
	std::stringstream ss(sName);
	std::string str;
	ss >> str;

	// debug
	std::cout << GREEN << "siteName = " << str << RESET << std::endl;

	return str;
}

static std::map<std::string, bool> createMethod(std::string str)
{
	// bool GET = false;
	// bool POST = false;
	// bool DELETE = false;

	std::map<std::string, bool> method;
	method.insert(std::make_pair(std::string("GET"), false));
	method.insert(std::make_pair(std::string("POST"), false));
	method.insert(std::make_pair(std::string("DELETE"), false));

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
	std::cout << BOLD << "Method :\n\t" << RESET;
	std::cout << GREEN << std::boolalpha << "Get: " << method.at("GET") << "\n\tPOST: " << method.at("POST") << "\n\tDELETE: " << method.at("DELETE") << RESET << std::endl;

	return method;
}

static std::vector<std::string> createRedirection(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string line;
	ss >> line;

	std::vector<std::string> ret;
	while (ss >> line)
	{
		ret.push_back(line);
	}
	// debug
	std::cout << BOLD << "Redirection contain: \n\t" << RESET << GREEN;
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); it++)
	{
		std::cout << *it << " - ";
	}
	std::cout << RESET << std::endl;

	return ret;
}

static std::string returnRoot(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	// debug
	std::cout << BOLD << "Second element of " << str << " is\n\t";
	std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

static std::string returnDefaultFile(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	std::ifstream stream(ret);
	if (!stream.is_open())
	{
		std::string error("Error: invalid variable in line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	std::cout << BOLD << "Second element of " << str << " is\n\t";
	std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

static bool returnBool(std::string str)
{
	removeSemiColon(str);

	std::stringstream ss(str);
	std::string word;
	int i = 0;
	for (; ss >> word; i++)
	{	
	}
	if (i != 2)
	{
		std::string error("Error: invalid number of variable in line:\n\t");
		throw std::invalid_argument(error + str);
	}

	if (str.find("true") != std::string::npos)
		return true;
	else if (str.find("false") != std::string::npos)
		return false;

	std::string error("Error: invalid variable in line:\n\t");
	throw std::invalid_argument(error + str);
}

static bool endsWith(const std::string& fullString, const std::string& ending)
{
    if (ending.size() > fullString.size())
        return false;

    return fullString.compare(fullString.size()
                              - ending.size(),
                              ending.size(), ending) == 0;
}

static std::string returnCGI(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	if (!endsWith(str, std::string(".php")) &&
		!endsWith(str, std::string(".py")) &&
		!endsWith(str, std::string(".sh")))
	{
		std::string error("Error: invalid codec file on line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	std::cout << BOLD << "Second element of " << str << " is\n\t";
	std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

static site createStructSite(siteParse site)
{
	struct site ret;

	ret.method = createMethod(site.method);
	ret.redirection = createRedirection(site.redirection);
	ret.dirRoot = returnRoot(site.dirRoot);
	ret.dirListing = returnBool(site.dirListing);
	ret.defaultFile = returnDefaultFile(site.defaultFile);
	ret.uploadFiles = returnBool(site.uploadFiles);
	ret.CGI = returnCGI(site.CGI);

	return ret;
}

void Config::siteParsing(siteParse site)
{
	std::string siteName = getSiteName(site.siteName);
	struct site siteStruct = createStructSite(site);

	sites.insert(std::make_pair(siteName, siteStruct));
}


int main()
{
	structParse data;
	siteParse site;

	site.siteName = std::string("demo {");
	site.method = std::string("methods GET POST DELETE;");
	site.redirection = std::string("redirection www.localhost pipi;");
	site.dirRoot = std::string("root /www/;");
	site.dirListing = std::string("listDirectory true;");
	site.defaultFile = std::string("defaultFile ./www/demo/index.html;");
	site.uploadFiles = std::string("uploadingFile true;");
	site.CGI = std::string("CGI /path/of/cgi/CGI.php;");

	data.site.push_back(site);

	try
	{
		Config salut(data);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}
