
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
	if (configStruct.address.empty())
		throw(std::invalid_argument("missing address!"));
}


std::string addElem(std::string line, std::string elem)
{
	std::cout << BLUE << "in addElem, line: " << line << RESET << std::endl;
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line);
}

void addVectString(std::string line, structParse *configStruct)
{
	static unsigned int iPort = 0;
	static unsigned int iAdd = 0;
	static unsigned int iHostName = 0;
	
	std::cout << "in addvectString" << std::endl;
	if (!line.compare(0, 7, "address"))
	{
		if (configStruct->port.size() <= iAdd)
			configStruct->port.push_back("4242");
		if (configStruct->hostname.size() <= iAdd)
			configStruct->hostname.push_back("perfectServer");
		configStruct->address.push_back(line);
		iAdd++;
		return ;
	}
	else if (!line.compare(0, 4, "port"))
	{
		if (configStruct->address.size() <= iPort)
			configStruct->address.push_back("localhost");
		if (configStruct->hostname.size() <= iPort)
			configStruct->hostname.push_back("perfectServer");
		configStruct->port.push_back(line);
		iPort++;
		return ;
	}
	else if (!line.compare(0, 8, "hostname"))
	{
		if (configStruct->address.size() <= iHostName)
			configStruct->address.push_back("localhost");
		if (configStruct->port.size() <= iHostName)
			configStruct->port.push_back("4242");
		std::cout << "in hostanme condition" << std::endl;
		configStruct->hostname.push_back(line);
		iHostName++;
		return ;
	}
	throw (std::invalid_argument("Error with adding item in vector<std::string>"));	
}

void addNewSite(std::string line, structParse *configStruct, bool *inSite)
{
	static siteParse tempStruct;

	// TODO: Reflechir en foutre la tempStruct avec une adresse ou un pointeur (parce que la rien ne se mets dans la struct final avec push_back)
	std::cout << "	in addNewSite" << std::endl;
	tempStruct.siteName = line.substr(0, line.find(':') - 1);
	*inSite = 1;
	if (!line.compare(0, 5, "methods"))
		tempStruct.method = addElem(line, tempStruct.method);
	else if (!line.compare(0, 11, "redirection"))
		tempStruct.redirection = addElem(line, tempStruct.redirection);
	else if (!line.compare(0, 4, "root"))
		tempStruct.dirRoot = addElem(line, tempStruct.dirRoot);
	else if (!line.compare(0, 13, "listDirectory"))
		tempStruct.dirListing = addElem(line, tempStruct.dirListing);
	else if (!line.compare(0, 11, "defaultFile"))
		tempStruct.defaultFile = addElem(line, tempStruct.defaultFile);
	else if (!line.compare(0, 10, "uploadFile"))
		tempStruct.uploadFiles = addElem(line, tempStruct.uploadFiles);
	else if (!line.compare(0, 3, "CGI"))
		tempStruct.CGI = addElem(line, tempStruct.CGI);
	else if (!line.compare(0, 1, "}"))
	{
		std::cout << YELLOW << "	end of site" << RESET << std::endl;
		*inSite = 0;
		configStruct->site.push_back(tempStruct);
		std::cout << "test dirslisting in struct final" << configStruct->site.at(0).dirListing << std::endl;
		std::cout << "--- PRINT TEMPSTRUCT ---" << std::endl;
		std::cout << BOLD << "dirListing: " << RESET << tempStruct.dirListing << std::endl;
		std::cout << BOLD << ", defaultFile : " << RESET << tempStruct.defaultFile << std::endl;
		std::cout << BOLD << ", uploadFiles : " << RESET << tempStruct.uploadFiles << std::endl;
		std::cout << BOLD << ", CGI : " << RESET << tempStruct.CGI << std::endl;
		// clear tempStruct?
	}
}

void addLine(std::string line, structParse *configStruct)
{
	static bool inErrorPage = 0;
	static bool inSite = 0;

	rmWhiteSpaces(&line);
	std::cout << BLUE << "test, line: " << BOLD << line << RESET << std::endl;
	if (!line.compare(0, 7, "address") || !line.compare(0, 4, "port") || !line.compare(0, 8, "hostname"))
		addVectString(line, configStruct);
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
	else if ((line.find(" {", 0) > 0) || inSite == 1)// create fct newSite
		addNewSite(line, configStruct, &inSite);
	else if (!line.compare(0, 6, "server"))
		return;
	else if (!line.compare(0, 1, "{"))
		return;
	else
		throw(std::invalid_argument("Invalid input: " + line));
}

struct structParse createStruct(std::string configPath)
{
	// TODO: verif si un des 3 vecto, il faut les 3 ou mettre des valeurs par default
	structParse configStruct;
	siteParse temp;
	std::ifstream configFile;
	std::string line;

	configFile.open(configPath.c_str());
	configStruct.site.push_back(temp);
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
		structTest = createStruct("../../config.conf");
	} catch (const std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return (1);
	}

	std::cout << GREEN << "The struct:" << RESET << std::endl;
	std::cout << BOLD << "address: " << RESET << structTest.address.at(0) << std::endl;
	std::cout << BOLD << "port: " << RESET << structTest.port[0] << std::endl;
	std::cout << BOLD << "errorCode: " << RESET << structTest.errorCode << std::endl;
	std::cout << BOLD << "errorPath: " << RESET << structTest.errorPath << std::endl;
	std::cout << BOLD << "maxSize: " << RESET << structTest.maxSize << std::endl;
	for (int i = 0; i < 1; i++)
	{
		std::cout << BOLD << "site " << i << ", siteName : " << RESET << structTest.site[i].siteName << std::endl;
		std::cout << BOLD << "site " << i << ", method : " << RESET << structTest.site[i].method << std::endl;
		std::cout << BOLD << "site " << i << ", redirection : " << RESET << structTest.site[i].redirection << std::endl;
		std::cout << BOLD << "site " << i << ", dirRoot : " << RESET << structTest.site[i].dirRoot << std::endl;
		std::cout << BOLD << "site " << i << ", dirListing : " << RESET << structTest.site[i].dirListing << std::endl;
		std::cout << BOLD << "site " << i << ", defaultFile : " << RESET << structTest.site[i].defaultFile << std::endl;
		std::cout << BOLD << "site " << i << ", uploadFiles : " << RESET << structTest.site[i].uploadFiles << std::endl;
		std::cout << BOLD << "site " << i << ", CGI : " << RESET << structTest.site[i].CGI << std::endl;
	}
}

// *** BACKUP ***

//std::string addElemTab(std::string line, std::string elem, std::ifstream *fileConfig)
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
//}
