#include "../../includes/configStruct.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/printDebug.hpp"
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include "../../includes/Location.hpp"

struct bracketData {
	bool inServer ;
	bool inLocation ;
};

typedef void (*directiveHandler)(std::string &line, server *srv, bracketData *brackets);

std::string extractDirective(std::string &line)
{
	std::stringstream ss(line);
	std::string ret;

	ss >> ret;
	return (ret);
}

void rmWhiteSpaces(std::string *line)
{
	if (line->empty())
		return ;
	while (line->at(0) == '	' || line->at(0) == ' ')
	{
		line->erase(0, 1);
	}
}

static void updateServerBracket(std::string &line, server *srv, bracketData *brackets)
{
	(void)line;
	(void)srv;

	if (brackets->inServer)
		brackets->inServer = false;
	else
		brackets->inServer = true;
}

static void updateLocationBracket(std::string &line, server *srv, bracketData *brackets)
{
	static unsigned int locI = -1;

	if (brackets->inLocation)
		brackets->inLocation = false;
	else
	{
		locI++;
		brackets->inLocation = true;
		srv->locations.push_back(location());
		srv->locations.at(locI).path = line;
	}
}

static void addElem(std::string &line, server *srv, bracketData *brackets)
{
	std::string directive = extractDirective(line);
	unsigned int LocI = srv->locations.size() - 1;

	if (brackets->inLocation)
	{
		if (!directive.compare(0, 9, "autoIndex"))
			srv->locations[LocI].autoIndex = line;
		else if (!directive.compare(0, 13, "method"))
			srv->locations[LocI].allowedMethods = line;
		else if (!directive.compare(0, 5, "index"))
			srv->locations[LocI].index = line;
		else if (!directive.compare(0, 6, "return"))
			srv->locations[LocI].ret = line;
		else if (!directive.compare(0, 10, "uploadPath"))
			srv->locations[LocI].uploadPath = line;
		else
			throw std::invalid_argument("Error with the line: " + line);
	}
	else if (brackets->inServer)
	{
		if (!directive.compare(0, 8, "hostname"))
			srv->hostname = line;
		else if (!directive.compare(0, 4, "root"))
			srv->root = line;
		else if (!directive.compare(0, 7, "maxSize"))
			srv->maxSize = line;
		else
			throw std::invalid_argument("Error with the line: " + line);
	}
	else 
	{
			throw std::invalid_argument("Error with the line: " + line);
	}
}

static void addVect(std::string &line, server *srv, bracketData *brackets)
{
	std::string directive = extractDirective(line);
	unsigned int locI = srv->locations.size() - 1;

	if (brackets->inLocation)
	{
		if (!directive.compare(0, 3, "cgi"))
			srv->locations[locI].cgi.push_back(line);
		else
			throw std::invalid_argument("Error, impossible to add this line: " + line);
		
	}
	else if (brackets->inServer)
	{
		if (!directive.compare(0, 6, "listen"))
			srv->listen.push_back(line);
		else if (!directive.compare(0, 10, "errorPage"))
			srv->errorPages.push_back(line);
		else
			throw std::invalid_argument("Error, impossible to add this line: " + line);
	}
	else
			throw std::invalid_argument("Error, impossible to add this line: " + line);
}

static std::map<std::string, directiveHandler > createDispatchTable()
{
	std::map<std::string, directiveHandler> dispatchTable;

	dispatchTable["server"] = &updateServerBracket;
	dispatchTable["hostname"] = &addElem;
	dispatchTable["listen"] = &addVect;
	dispatchTable["root"] = &addElem;
	dispatchTable["maxSize"] = &addElem;
	dispatchTable["errorPage"] = &addVect;
	dispatchTable["location"] = &updateLocationBracket;
	dispatchTable["autoIndex"]= &addElem;
	dispatchTable["method"] = &addElem;
	dispatchTable["index"] = &addElem;
	dispatchTable["return"] = &addElem;
	dispatchTable["cgi"] = &addVect;
	dispatchTable["uploadPath"] = &addElem;
	dispatchTable["path"] = &addElem;
	

	return (dispatchTable);

}

void checkEmptyElem(server *serverStruct)
{
	(void)serverStruct;
	return ;
}


static void addLine(std::string line, server *srv, bracketData *brackets, std::size_t cline, std::map<std::string, directiveHandler> dispatchTable)
{
	std::map<std::string, directiveHandler>::iterator it = dispatchTable.begin();
	std::map<std::string, directiveHandler>::iterator ite = dispatchTable.end();
	std::string directive = extractDirective(line);
	std::stringstream numLine;

	numLine	<< cline;
	rmWhiteSpaces(&line);
	if (line.empty())
		return ;
	while (it != ite)
	{
		if (directive.find(it->first) < line.size())
		{
			if (brackets->inServer || ((directive.find("server") < directive.size() && line.find("{") < line.size())))
				it->second(line, srv, brackets);
			else
				throw std::invalid_argument("Directive need to be in a server bracket! (at line: " + numLine.str());
			return ;	
		}
		if (directive.find("}") < line.size())
		{
			if (brackets->inLocation)
				brackets->inLocation = false;
			else if (brackets->inServer)
				brackets->inServer = false;
			return ;
		}
		it++;
	}
	throw std::invalid_argument("Invalid directive: " + directive);
}

server CreateStruct(std::string configPath)
{
	server configStruct;
	std::ifstream configFile;
	std::string line;
	bracketData brackets;
	std::size_t cLine;
	std::map<std::string, directiveHandler> dispatchTable;

	configFile.open(configPath.c_str());
	if (!configFile.is_open())
		throw std::invalid_argument("Error, invalid path for the config file");
	dispatchTable = createDispatchTable();
	cLine = 1;
	while (std::getline(configFile, line))
	{
		addLine(line, &configStruct, &brackets, cLine, dispatchTable);
		cLine++;
	}
	configFile.close();
	//checkEmptyElem(&configStruct);
	if (brackets.inServer)
		throw(std::invalid_argument("Error, missing closing bracket '}' at the end of file"));
	return (configStruct);

}

int main(void)
{
	try 
	{
		server testStruct = CreateStruct("../../newGood.conf");
		Location test(testStruct.locations[0], "./www/danalexian/");
		// printStructV2(testStruct);
		test.print();
	} 
	catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
}
