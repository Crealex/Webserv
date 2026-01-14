#include "../../includes/colors.hpp"
#include "../../includes/structParse.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>

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
 * @brief check if all site attributs is full
 *
 * @param temp a copy of siteParse
 * @return
 */
bool isFull(siteParse temp)
{
	if (temp.siteName.empty() || temp.CGI.empty() || temp.defaultFile.empty() || temp.uploadFiles.empty() || temp.dirListing.empty() || temp.dirRoot.empty() || temp.method.empty() || temp.redirection.empty())
		return false;
	return true;
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
		throw(std::invalid_argument("Error, missing maxSize"));
	for (long unsigned int i = 0; i < configStruct->vServer.size(); i++)
	{
		if (configStruct->vServer[i].addressPort.size() == 0)
			configStruct->vServer[i].addressPort.push_back("listen localhost:4242 (default)");
	}
	if (configStruct->site.size() == 0)
		throw(std::invalid_argument("Error, missing site!"));
	for (long unsigned int i = 0; i < configStruct->site.size(); i++)
	{
		std::stringstream ss;
		ss << i;
		if (configStruct->site[i].siteName.empty())
			throw(std::invalid_argument("Error, missing siteName for the site no " + ss.str()));
		if (configStruct->site[i].CGI.empty())
			throw(std::invalid_argument("Error, missing CGI for the site no " + ss.str()));
		if (configStruct->site[i].dirRoot.empty())
			throw(std::invalid_argument("Error, missing root for the site no " + ss.str()));
		if (configStruct->site[i].method.empty())
			throw(std::invalid_argument("Error, missing methods for the site no " + ss.str()));
		if (configStruct->site[i].defaultFile.empty())
			throw(std::invalid_argument("Error, missing defaultFile for the site no " + ss.str()));
		if (configStruct->site[i].dirListing.empty())
			configStruct->site[i].dirListing = "listDirectory false (default)";
		if (configStruct->site[i].uploadFiles.empty())
			configStruct->site[i].uploadFiles = "uploadingFile false (default)";
	}
}

/**
 * @brief add an element to configStruct and check if is a duplicate
 *
 * @param line the current line
 * @param elem the current element
 * @return a string to put in the configStruct
 */
std::string addElem(std::string line, std::string elem)
{
	if (!elem.empty())
		throw std::invalid_argument(elem + " already exist!");
	return (line);
}

/**
 * @brief Add element in the struct hostname (named vServer)
 *
 * @param line: The current line
 * @param configStruct: A pointer to the main structParse
 */
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
		for (unsigned int i = 0; i < configStruct->vServer.size(); i++)
			if (line == configStruct->vServer[i].serverName)
				throw(std::invalid_argument("Error, " + line + " already exist"));
		iHostName++;
		configStruct->vServer.push_back(hostname());
		configStruct->vServer[iHostName].serverName = line;
		return;
	}
	throw(std::invalid_argument("Error with adding item in vector<std::string>, line: " + line));
}

/**
 * @brief Add all attributs of a site
 *
 * @param line: The current line
 * @param configStruct A pointer to the main structParse
 * @param inSite: A boolean to now if we are in the site bracket or not
 * @param cLine: The number of the current line
 */
void addNewSite(std::string line, structParse *configStruct, bool *inSite, std::string cLine)
{
	static siteParse tempStruct;

	if (*inSite == 0)
	{
		for (unsigned int i = 0; i < configStruct->site.size(); i++)
		{
			if (line == configStruct->site[i].siteName)
				throw(std::invalid_argument("Error, at line " + cLine + ", site name already exist"));
		}
		tempStruct.siteName = line;
		if (tempStruct.siteName.length() <= 2)
			throw(std::invalid_argument("Error, needed a siteName before the open bracket '{'"));
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
	} else if (isFull(tempStruct))
		throw(std::invalid_argument("Error, needed to close the site segment with '}'"));
	else
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

	ss << cLine;
	if (line.empty())
		throw(std::invalid_argument("Error, empty line at line " + ss.str()));
	rmWhiteSpaces(&line);
	if (!*inServer && cLine != 1)
		throw(std::invalid_argument("Error, no elements after the end bracket '}' is permise"));
	if (line.compare(0, 6, "server") && cLine == 1)
		throw(std::invalid_argument("Error, missing server opening bracket at the begining of file 'server {'"));
	if ((!line.compare(0, 6, "listen") || !line.compare(0, 8, "hostname")) && *inServer)
		addVectString(line, configStruct);
	else if (!line.compare(0, 6, "server"))
		*inServer = 1;
	else if (!line.compare(0, 9, "errorPage") && *inServer)
		configStruct->ErrorPage.push_back(line);
	else if (!line.compare(0, 7, "maxSize") && *inServer)
		configStruct->maxSize = addElem(line, configStruct->maxSize);
	else if (((line.find(" {", 0) <= line.size()) || inSite == 1) && *inServer)
		addNewSite(line, configStruct, &inSite, ss.str());
	else if (!line.compare(0, 1, "}") && *inServer)
		*inServer = 0;
	else if (line.find("{", 0) <= line.size())
		throw(std::invalid_argument("Error, needed a space and a name before '{' at line " + ss.str()));
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
	if (!configFile.is_open())
		throw std::invalid_argument("Error, invalid path for the config file");
	inServer = 0;
	cLine = 1;
	while (1)
	{
		std::getline(configFile, line);
		if (line == "\0")
			break;
		if (line.empty())
			throw(std::invalid_argument("Error, empty line in the file"));

		addLine(line, &configStruct, &inServer, cLine);
		cLine++;
	}
	configFile.close();
	checkEmptyElem(&configStruct);
	if (inServer)
		throw(std::invalid_argument("Error, missing closing bracket '}' at the end of file"));
	return (configStruct);
}
