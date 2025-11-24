#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Clear the tempStruct in addNewSite() (beacause it's a static variable)
 *
 * @param temp The struct to clear
 */
void clearStruct(siteParse *temp)
{
	temp->method.clear();
	temp->dirListing.clear();
	temp->siteName.clear();
	temp->CGI.clear();
	temp->defaultFile.clear();
	temp->uploadFiles.clear();
	temp->dirRoot.clear();
	temp->redirection.clear();
}

/**
 * @brief Remove the whites spaces before the line
 *
 * @param line The current line
 */
void rmWhiteSpaces(std::string *line)
{
	while (line->at(0) == '	' || line->at(0) == ' ')
	{
		line->erase(0, 1);
	}
}

/**
 * @brief Check if a mandatory element is missing and throw a error or put a default value
 *
 * @param configStruct The structure of the configartion file
 */
void checkEmptyElem(struct structParse *configStruct)
{
	if (configStruct->maxSize.empty())
		throw(std::invalid_argument("missing maxSize!"));
	for (long unsigned int i = 0; i < configStruct->vServer.size(); i++)
	{
		if (configStruct->vServer[i].addressPort.size() == 0)
			configStruct->vServer[i].addressPort.push_back("listen localhost:4242 (default)");
	}
}

std::string addElem(std::string line, std::string elem)
{
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line);
}

void addVectString(std::string line, structParse *configStruct)
{
	static unsigned int iHostName = -1;

	if (!line.compare(0, 6, "listen"))
	{
		if (configStruct->vServer.size() < iHostName)
			throw(std::invalid_argument("missing hostname berfore listen tag!"));
		configStruct->vServer[iHostName].addressPort.push_back(line);
		return;
	} else if (!line.compare(0, 8, "hostname"))
	{
		iHostName++;
		configStruct->vServer.push_back(hostname());
		configStruct->vServer[iHostName].serverName = line;
		return;
	}
	throw(std::invalid_argument("Error with adding item in vector<std::string>, line: " + line));
}

void addNewSite(std::string line, structParse *configStruct, bool *inSite, std::string cLine)
{
	static siteParse tempStruct;

	if (*inSite == 0)
	{
		tempStruct.siteName = line.substr(0, line.find(':') - 1);
		*inSite = 1;
		return;
	}
	if (!line.compare(0, 7, "methods"))
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
		*inSite = 0;
		configStruct->site.push_back(tempStruct);
		clearStruct(&tempStruct);
	} else
		throw(std::invalid_argument("invalid element at the line " + cLine + ": " + line));
}

/**
 * @brief Hub function for choose which fct call and clean a least the line
 *
 * @param line the current line
 * @param configStruct a pointer to the main struct
 * @param inServer a boolean to know when we are in the server bracket or not
 * @param cLine the number of the current line (for debug and error handling)
 */
void addLine(std::string line, structParse *configStruct, bool *inServer, unsigned int cLine)
{
	static bool inSite = 0;
	std::stringstream ss;

	// TODO:	[x] Check si on est bien dans une balise server pour les arguments necessaire
	//			[ ] check si bien balise fermante pour les sites
	//			[ ] check si il y a bien un mot avant une balise ouvrante
	//			[ ] check les elements indispensables dans les balises sites (voir avec les autres)
	rmWhiteSpaces(&line);
	if (line.compare(0, 6, "server") && cLine == 0)
		throw(std::invalid_argument("Error, missing server opening bracket at the begining of file 'server {'"));
	ss << cLine;
	if ((!line.compare(0, 6, "listen") || !line.compare(0, 8, "hostname")) && *inServer)
		addVectString(line, configStruct);
	else if (!line.compare(0, 6, "server"))
		*inServer = 1;
	else if (!line.compare(0, 9, "errorPage") && *inServer)
		configStruct->ErrorPage.push_back(line);
	else if (!line.compare(0, 7, "maxSize") && *inServer)
		configStruct->maxSize = addElem(line, configStruct->maxSize);
	else if (((line.find(" {", 0) < line.size()) || inSite == 1) && *inServer) // TODO: Demander a Dana se que retourne find en cas de non trouvage
		addNewSite(line, configStruct, &inSite, ss.str());
	else if (line.compare(0, 2, " }") && *inServer)
		*inServer = 0;
	else
		throw(std::invalid_argument("Invalid input at line " + ss.str() + ": " + line));
}

/**
 * @brief Function to call for creating the struct for the config file
 *
 * @param configPath The path of the config file
 * @return The sruct "structParse" with the good elements
 */
struct structParse createStruct(std::string configPath)
{
	structParse configStruct;
	std::ifstream configFile;
	std::string line;
	bool inServer;
	unsigned int cLine;

	configFile.open(configPath.c_str());
	inServer = 0;
	cLine = 0;
	while (1)
	{
		std::getline(configFile, line, '\n');
		if (line.empty())
			break;
		addLine(line, &configStruct, &inServer, cLine);
		cLine++;
	}
	configFile.close();
	checkEmptyElem(&configStruct);
	if (inServer)
		throw(std::invalid_argument("Error, missing closing bracket '}' at the end of file"));
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
	std::cout << BOLD << "maxSize: " << RESET << structTest.maxSize << std::endl;
	std::cout << BLUE << "All hostnames:" << RESET << std::endl;
	for (long unsigned int i = 0; i < structTest.vServer.size(); i++)
	{
		std::cout << BOLD << "	hostname " << i << ": " << RESET << structTest.vServer[i].serverName << std::endl;
		for (long unsigned int j = 0; j < structTest.vServer[i].addressPort.size(); j++)
			std::cout << "		listen: " << structTest.vServer[i].addressPort[j] << std::endl;
	}
	std::cout << BLUE << "errorPage:" RESET << std::endl;
	for (long unsigned int i = 0; i < structTest.ErrorPage.size(); i++)
		std::cout << BOLD << "	" << i << RESET << ": " << structTest.ErrorPage[i] << std::endl;
	for (long unsigned int i = 0; i < structTest.site.size(); i++)
	{
		std::cout << BLUE << "site " << i << ": " << RESET << std::endl;
		std::cout << BOLD << "	siteName : " << RESET << structTest.site[i].siteName << std::endl;
		std::cout << BOLD << "	methods : " << RESET << structTest.site[i].method << std::endl;
		std::cout << BOLD << "	redirection : " << RESET << structTest.site[i].redirection << std::endl;
		std::cout << BOLD << "	dirRoot : " << RESET << structTest.site[i].dirRoot << std::endl;
		std::cout << BOLD << "	dirListing : " << RESET << structTest.site[i].dirListing << std::endl;
		std::cout << BOLD << "	defaultFile : " << RESET << structTest.site[i].defaultFile << std::endl;
		std::cout << BOLD << "	uploadFiles : " << RESET << structTest.site[i].uploadFiles << std::endl;
		std::cout << BOLD << "	CGI : " << RESET << structTest.site[i].CGI << std::endl;
	}
}

// *** BACKUP ***
