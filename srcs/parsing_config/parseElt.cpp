#include <sstream>
#include <fstream>
#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include "../../includes/Config.hpp"

/**
 * @brief Check if there is a semicolon at the end of the string 
 * and erase it, if not throw an error
 * 
 * @param str 
 */
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

/**
 * @brief Check if there is exactly two element in the string.
 * the different element are separate with space.
 * if the number of element is not 2 throw an error
 * 
 * @param str 
 */
static void twoElement(std::string str)
{
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
}

/**
 * @brief Get the Site Name object
 * 
 * @param sName 
 * @return std::string 
 */
static std::string getSiteName(std::string sName)
{
	std::stringstream ss(sName);
	std::string str;
	ss >> str;

	// debug
	//std::cout << GREEN << "siteName = " << str << RESET << std::endl;

	return str;
}

/**
 * @brief Create a Method object.
 * Create the map and fill it with the our 3 method/bool pair.
 * the bool are by default false and if the method is found in the string
 * put the right bool at true. if an unkown method is found in the string
 * return an error
 * 
 * @param str 
 * @return std::map<std::string, bool> 
 */
static std::map<std::string, bool> createMethod(std::string str)
{
	std::map<std::string, bool> method;
	method.insert(std::make_pair(std::string("GET"), false));
	method.insert(std::make_pair(std::string("POST"), false));
	method.insert(std::make_pair(std::string("DELETE"), false));

	removeSemiColon(str);
	std::stringstream ss(str);
	std::string word;
	int i = 0;
	ss >> word;
	while (ss >> word)
	{
		i = 1;
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
	if (i == 0)
	{
		std::string error("Error: no method are specified in line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	//std::cout << BOLD << "Method :\n\t" << RESET;
	//std::cout << GREEN << std::boolalpha << "Get: " << method.at("GET") << "\n\tPOST: " << method.at("POST") << "\n\tDELETE: " << method.at("DELETE") << RESET << std::endl;

	return method;
}

/**
 * @brief Create a Redirection object.
 * Get each element after the first in the string param
 * and push them in the returned vector
 * 
 * @param str 
 * @return std::vector<std::string> 
 */
static std::vector<std::string> createRedirection(std::string str)
{
	removeSemiColon(str);
	std::stringstream ss(str);
	std::string line;
	ss >> line;

	std::vector<std::string> ret;
	int i = 0;
	while (ss >> line)
	{
		i = 1;
		ret.push_back(line);
	}
	if (i == 0)
	{
		std::string error("Error: no Redirection on line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	//std::cout << BOLD << "Redirection contain: \n\t" << RESET << GREEN;
	//for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); it++)
	//{
	//	std::cout << *it << " - ";
	//}
	//std::cout << RESET << std::endl;

	return ret;
}

static std::string returnRoot(std::string str)
{
	removeSemiColon(str);
	twoElement(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	// debug
	//std::cout << BOLD << "Second element of " << str << " is\n\t";
	//std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

/**
 * @brief Return the default file path and check if 
 * it's a valid path, if not throw an error
 * 
 * @param str 
 * @return std::string 
 */
static std::string returnDefaultFile(std::string str)
{
	removeSemiColon(str);
	twoElement(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	std::ifstream stream(ret.c_str());
	if (!stream.is_open())
	{
		std::string error("Error: invalid variable in line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	//std::cout << BOLD << "Second element of " << str << " is\n\t";
	//std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

/**
 * @brief Check if there is exactly 2 element
 * and return is the second element is "true" or "false"
 * 
 * @param str 
 * @return true 
 * @return false 
 */
static bool returnBool(std::string str)
{
	removeSemiColon(str);
	twoElement(str);

	if (str.find("true") != std::string::npos)
		return true;
	else if (str.find("false") != std::string::npos)
		return false;

	std::string error("Error: invalid variable in line:\n\t");
	throw std::invalid_argument(error + str);
}

/**
 * @brief Check if the fullString param end with the
 * ending string, return true if yes and false if not
 * 
 * @param fullString 
 * @param ending 
 * @return true 
 * @return false 
 */
static bool endsWith(const std::string& fullString, const std::string& ending)
{
    if (ending.size() > fullString.size())
        return false;

    return fullString.compare(fullString.size()
                              - ending.size(),
                              ending.size(), ending) == 0;
}

/**
 * @brief Check if the path of the CGI end with the right
 * codec for a CGI (.php, .py, etc)
 * 
 * @param str 
 * @return std::string 
 */
static std::string returnCGI(std::string str)
{
	removeSemiColon(str);
	twoElement(str);
	std::stringstream ss(str);
	std::string ret;

	ss >> ret;
	ss >> ret;
	
	if (access(ret.c_str(), F_OK) == -1)
	{
		std::string error("Error: invalid file path:\n\t");
		throw std::invalid_argument(error + str);
	}
	if (!endsWith(str, std::string(".php")) &&
		!endsWith(str, std::string(".py")) &&
		!endsWith(str, std::string(".sh")))
	{
		std::string error("Error: invalid extension file on line:\n\t");
		throw std::invalid_argument(error + str);
	}

	// debug
	//std::cout << BOLD << "Second element of " << str << " is\n\t";
	//std::cout << RESET << GREEN << str << " return -> " << ret << RESET << std::endl;
	
	return ret;
}

/**
 * @brief Create a Struct Site object. 
 * Get each element of the siteParse struct, check them
 * and parse them correctly in the return site
 * 
 * @param site 
 * @return site 
 */
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

	this->sitesName.push_back(siteName);
	this->sites.insert(std::make_pair(siteName, siteStruct));
}

// int main()
// {
// 	structParse data;
// 	siteParse site;

// 	site.siteName = std::string("demo {");
// 	site.method = std::string("methods GET POST DELETE;");
// 	site.redirection = std::string("redirection www.localhost pipi;");
// 	site.dirRoot = std::string("root /www/;");
// 	site.dirListing = std::string("listDirectory true;");
// 	site.defaultFile = std::string("defaultFile www/demo/index.html;");
// 	site.uploadFiles = std::string("uploadingFile true;");
// 	site.CGI = std::string("CGI /path/of/cgi/CGI.php;");

// 	data.site.push_back(site);

// 	try
// 	{
// 		Config salut(data);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << RED << e.what() << '\n';
// 	}
	
// }
