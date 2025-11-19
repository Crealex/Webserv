
#include "../../includes/structParse.hpp"
#include "../../includes/colors.hpp"
#include <fstream>
#include <stdexcept>
#include <string>


void rmWhiteSpaces(std::string *line)
{
	int i;

	i = 0;
	while (line->at(i) == '\t' || line->at(i) == ' ')
	{
		line->erase(i, 1);
		i++;
	}
}

std::string addSiteName(std::string line, std::string elem, bool *inSite)
{
	*inSite = 1;
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line.substr(0, line.find(':') - 1));
}

void checkEmptyElem(struct structParse configStruct)
{
	if (configStruct.address.empty())
		throw (std::invalid_argument("missing address!"));
}

std::string addElemTab(std::string line, std::string elem, std::ifstream *fileConfig)
{
	std::string ret;
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	while (line.find("]"))
	{
		std::getline(*fileConfig, line, '\n');
		if (line.find("]"))
			break;
		if (ret.empty())
			ret = line;
		else
			ret += ", " + line;
	}
	return (ret);
}

std::string addElem(std::string line, std::string elem)
{
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line);
}

void addLine(std::string line, struct structParse *configStruct, std::ifstream *configFile)
{
	static bool inErrorPage = 0;
	static bool inSite = 0;
	static int	iSite = 0;

	rmWhiteSpaces(&line);
	if (!line.compare(0, 4, "host"))
		configStruct->address = addElem(line, configStruct->address);
	else if (!line.compare(0, 4, "port"))
		configStruct->port = addElem(line, configStruct->port);
	else if (!line.compare(0, 10, "Error_page"))
		inErrorPage = 1;
	else if (!line.compare(0, 4, "code") && inErrorPage) // a revoir
		configStruct->errorCode = addElem(line, configStruct->errorCode);
	else if (!line.compare(0, 4, "path") && inErrorPage) // a revoir
		configStruct->errorPath = addElem(line, configStruct->errorPath);
	else if (!line.compare(0, 7, "maxSize"))
		configStruct->maxSize = addElem(line, configStruct->maxSize);
	else if (line.find(": {", 0) != -1)
		configStruct->site[iSite].siteName =  addSiteName(line, configStruct->site[iSite].siteName, &inSite);
	else if (!line.compare(0, 5, "methods"))
		configStruct->site[iSite].method = addElemTab(line, configStruct->site[iSite].method, configFile);
	else if (!line.compare(0, 11, "redirection"))
		configStruct->site[iSite].redirection = addElem(line, configStruct->site[iSite].redirection);
	else if (!line.compare(0, 4, "root"))
		configStruct->site[iSite].dirRoot = addElem(line, configStruct->site[iSite].dirRoot);
	else if (!line.compare(0, 10, "dirListing"))
		configStruct->site[iSite].dirListing = addElem(line, configStruct->site[iSite].dirListing);
	else if (!line.compare(0, 11, "defaultFile"))
		configStruct->site[iSite].defaultFile = addElem(line, configStruct->site[iSite].defaultFile);
	else if (!line.compare(0, 10, "uploadFile"))
		configStruct->site[iSite].uploadFiles = addElem(line, configStruct->site[iSite].uploadFiles);
	else if (!line.compare(0, 3, "CGI"))
		configStruct->site[iSite].CGI = addElem(line, configStruct->site[iSite].CGI);
	else if (!line.compare(0, 1, "}") && inErrorPage)
		inErrorPage = 0;
	else if (!line.compare(0, 1, "}") && inSite)
	{
		inSite = 0;
		iSite++;
	}
	else
		throw (std::invalid_argument("Invalid input: " + line));
	
}

struct structParse createStruct(std::string configPath)
{
	struct structParse configStruct;
	std::ifstream configFile;
	std::string line;
	
	configFile.open(configPath);
	while (1)
	{
		std::getline(configFile, line, '\n');
		if (line.empty())
			break;
		addLine(line, &configStruct, &configFile);
	}
	configFile.close();
	checkEmptyElem(configStruct);
	return (configStruct);
}


// *** MAIN DE TEST ***

int main(void)
{
	try 
	{
		createStruct("../../config.json");
	}
	catch (const std::exception &e) 
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
}
