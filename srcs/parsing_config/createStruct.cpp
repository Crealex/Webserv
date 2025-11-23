
#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

void rmWhiteSpaces(std::string *line)
{
	while (line->at(0) == '	' || line->at(0) == ' ')
	{
		line->erase(0, 1);
	}
}

void checkEmptyElem(struct structParse configStruct)
{
	if (configStruct.maxSize.empty())
		throw(std::invalid_argument("missing maxSize!"));
}

std::string addElem(std::string line, std::string elem)
{
	// std::cout << BLUE << "in addElem, line: " << line << RESET << std::endl;
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line);
}

void addVectString(std::string line, structParse *configStruct)
{
	static unsigned int iHostName = 0;

	// std::cout << "in addvectString" << std::endl;
	if (!line.compare(0, 6, "listen"))
	{
		if (configStruct->hostname.size() <= iHostName)
			throw (std::invalid_argument("missing hostname berfore listen tag!"));
		configStruct->hostname[iHostName].addressPort.push_back(line);
	}
	else if (!line.compare(0, 8, "hostname"))
	{
		//verif que celui d'avant possede bien au moins une balise listen sinon en rajouter par default
		if (iHostName <= 0 && configStruct->hostname[iHostName - 1].addressPort.size() <= 0)
			configStruct->hostname[iHostName - 1].addressPort.push_back("localhost:4242");
		configStruct->hostname[iHostName].serverName = line;
		iHostName++;
		return;
	}
	throw(std::invalid_argument("Error with adding item in vector<std::string>"));
}

void addNewSite(std::string line, structParse *configStruct, bool *inSite)
{
	static siteParse tempStruct;

	if (*inSite == 0)
	{
		tempStruct.siteName = line.substr(0, line.find(':') - 1);
		*inSite = 1;
		return;
	}
	if (!line.compare(0, 8, "methods"))
		tempStruct.method = addElem(line, tempStruct.method);
	else if (!line.compare(0, 11, "redirection"))
		tempStruct.redirection = addElem(line, tempStruct.redirection);
	else if (!line.compare(0, 4, "root"))
		tempStruct.dirRoot = addElem(line, tempStruct.dirRoot);
	else if (!line.compare(0, 13, "listDirectory"))
		tempStruct.dirListing = addElem(line, tempStruct.dirListing);
	else if (!line.compare(0, 11, "defaultFile"))
		tempStruct.defaultFile = addElem(line, tempStruct.defaultFile);
	else if (!line.compare(0, 13, "uploadingFile"))
		tempStruct.uploadFiles = addElem(line, tempStruct.uploadFiles);
	else if (!line.compare(0, 3, "CGI"))
		tempStruct.CGI = addElem(line, tempStruct.CGI);
	else if (!line.compare(0, 1, "}"))
	{
		// std::cout << YELLOW << "	end of site" << RESET << std::endl;
		*inSite = 0;
		configStruct->site.push_back(tempStruct);
		// std::cout << "test dirslisting in struct final: " << configStruct->site[0].dirListing << std::endl;
		// std::cout << "size of struct siteParse: " << configStruct->site.size() << std::endl;
		// std::cout << "--- PRINT TEMPSTRUCT ---" << std::endl;
		// std::cout << BOLD << "siteName: " << RESET << tempStruct.siteName << std::endl;
		// std::cout << BOLD << "protocol: " << RESET << tempStruct.method << std::endl;
		// std::cout << BOLD << "redirection: " << RESET << tempStruct.redirection << std::endl;
		// std::cout << BOLD << "root: " << RESET << tempStruct.dirRoot << std::endl;
		// std::cout << BOLD << "dirListing: " << RESET << tempStruct.dirListing << std::endl;
		// std::cout << BOLD << "defaultFile: " << RESET << tempStruct.defaultFile << std::endl;
		// std::cout << BOLD << "uploadFiles: " << RESET << tempStruct.uploadFiles << std::endl;
		// std::cout << BOLD << "CGI: " << RESET << tempStruct.CGI << std::endl;
		// clear tempStruct?
	} else
		throw(std::invalid_argument("invalid element at the line: " + line));
}

void addLine(std::string line, structParse *configStruct)
{
	static bool inErrorPage = 0;
	static bool inSite = 0;

	rmWhiteSpaces(&line);
	// std::cout << BLUE << "test, line: " << BOLD << line << RESET << std::endl;
	if (!line.compare(0, 7, "address") || !line.compare(0, 4, "port") || !line.compare(0, 8, "hostname"))
		addVectString(line, configStruct);
	else if (!line.compare(0, 6, "server"))
		return;
	else if (!line.compare(0, 9, "errorPage"))
		inErrorPage = 1;
	else if (!line.compare(0, 4, "code") && inErrorPage) // a revoir
		configStruct->errorCode = addElem(line, configStruct->errorCode);
	else if (!line.compare(0, 4, "path") && inErrorPage) // a revoir
		configStruct->errorPath = addElem(line, configStruct->errorPath);
	else if (!line.compare(0, 1, "}") && inErrorPage)
		inErrorPage = 0;
	else if (!line.compare(0, 7, "maxSize"))
		configStruct->maxSize = addElem(line, configStruct->maxSize);
	else if ((line.find(" {", 0) > 0) || inSite == 1) // create fct newSite
		addNewSite(line, configStruct, &inSite);
	else if (!line.compare(0, 1, "{"))
		return;
	else
		throw(std::invalid_argument("Invalid input: " + line));
}

struct structParse createStruct(std::string configPath)
{
	structParse configStruct;
	std::ifstream configFile;
	std::string line;

	configFile.open(configPath.c_str());
	while (1)
	{
		std::getline(configFile, line, '\n');
		if (line.empty())
			break;
		addLine(line, &configStruct);
	}
	configFile.close();
	checkEmptyElem(configStruct);
	return (configStruct);
}

// *** MAIN DE TEST ***

int main(void)
{
	struct structParse structTest;

	try
	{
		structTest = createStruct("../../test.conf");
	} catch (const std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return (1);
	}

	std::cout << GREEN << "The struct:" << RESET << std::endl;
	std::cout << BLUE << "all addres:" << RESET << std::endl;
	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
	for (long unsigned int i = 0; i < structTest.hostname.size(); i++)
	{
		std::cout << BOLD << "	hostname " << i << ": "<< RESET << structTest.hostname[i].serverName << std::endl;
		for (long unsigned int j = 0; j < structTest.hostname[i].addressPort.size(); j++)
			std::cout << "		listen: " << structTest.hostname[i].addressPort[j] << std::endl;

	}
	std::cout << BOLD << "errorCode: " << RESET << structTest.errorCode << std::endl;
	std::cout << BOLD << "errorPath: " << RESET << structTest.errorPath << std::endl;
	std::cout << BOLD << "maxSize: " << RESET << structTest.maxSize << std::endl;
	for (long unsigned int i = 0; i < structTest.site.size(); i++)
	{
		std::cout << BOLD << "site " << i << ", siteName : " << RESET << structTest.site[i].siteName << std::endl;
		std::cout << BOLD << "site " << i << ", methods : " << RESET << structTest.site[i].method << std::endl;
		std::cout << BOLD << "site " << i << ", redirection : " << RESET << structTest.site[i].redirection << std::endl;
		std::cout << BOLD << "site " << i << ", dirRoot : " << RESET << structTest.site[i].dirRoot << std::endl;
		std::cout << BOLD << "site " << i << ", dirListing : " << RESET << structTest.site[i].dirListing << std::endl;
		std::cout << BOLD << "site " << i << ", defaultFile : " << RESET << structTest.site[i].defaultFile << std::endl;
		std::cout << BOLD << "site " << i << ", uploadFiles : " << RESET << structTest.site[i].uploadFiles << std::endl;
		std::cout << BOLD << "site " << i << ", CGI : " << RESET << structTest.site[i].CGI << std::endl;
	}
}

// *** BACKUP ***

// std::string addElemTab(std::string line, std::string elem, std::ifstream *fileConfig)
//{
//	std::string ret;
//	if (!elem.empty())
//		throw std::invalid_argument(elem + " already exist!");
//	while (line.find("]"))
//	{
//		std::getline(*fileConfig, line, '\n');
//		if (line.find("]"))
//			break;
//		if (ret.empty())
//			ret = line;
//		else
//			ret += ", " + line;
//	}
//	return (ret);
// }
