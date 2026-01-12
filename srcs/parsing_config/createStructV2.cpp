#include "../../includes/configStruct.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

struct bracketData {
	bool inServer = false;
	bool inLocation = false;
};

typedef void (*directiveHandler)(const std::string &line, server *srv, bracketData *brackets);


static void updateServerBracket(const std::string &line, server *srv, bracketData *brackets)
{
	if (brackets->inServer)
		brackets->inServer = false;
	else
		brackets->inServer = true;
}

static void updateLocationBracket(const std::string &line, server *srv, bracketData *brackets)
{
	unsigned int locI = srv->locations.size();

	if (brackets->inLocation)
		brackets->inLocation = false;
	else
	{
		brackets->inLocation = true;
		srv->locations.at(locI).path = line;
	}
}

static void addElem(const std::string &line, server *srv, bracketData *brackets)
{
	return;
}

static void addVect(const std::string &line, server *srv, bracketData *brackets)
{
	return;
}

static std::map<std::string, directiveHandler > createDispatchTable()
{
	std::map<std::string, directiveHandler> dispatchTable;

	dispatchTable["server"] = &updateServerBracket;
	dispatchTable["hostname"] = &addElem;
	dispatchTable["listen"] = &addVect;
	dispatchTable["root"] = &addElem;
	dispatchTable["maxSize"] = &addElem;
	dispatchTable["errorPages"] = &addVect;
	dispatchTable["location"] = &updateLocationBracket;
	dispatchTable["autoIndex"]= &addElem;
	dispatchTable["allowedMethods"] = &addElem;
	dispatchTable["index"] = &addElem;
	dispatchTable["return"] = &addElem;
	dispatchTable["cgi"] = &addVect;
	dispatchTable["uploadPath"] = &addElem;
	dispatchTable["path"] = &addElem;
	

	return (dispatchTable);

}

void checkEmptyElem(server *serverStruct)
{
	return ;
}

std::string extractDirective(std::string &line)
{
	std::stringstream ss(line);
	std::string ret;

	ss >> ret;
	return (ret);
}

static void addLine(std::string line, server *srv, bracketData *brackets, std::size_t cline, std::map<std::string, directiveHandler> dispatchTable)
{
	std::map<std::string, directiveHandler>::iterator it = dispatchTable.begin();
	std::map<std::string, directiveHandler>::iterator ite = dispatchTable.end();
	std::string directive = extractDirective(line);

	while (it != ite)
	{
		if (directive.find(it->first) < line.size())
		{
			if (directive.find("location") < directive.size() && line.find("{") < line.size())
				brackets->inLocation = true;
			if (directive.find("server") < directive.size() && line.find("{") < line.size())
				brackets->inServer = true;
			if (brackets->inServer)
				it->second(line, srv, brackets);
			else
				throw std::invalid_argument("Directive need to be in a server bracket!");
			return ;	
		}
		if (directive.find("}") < line.size())
		{
			if (brackets->inLocation)
				brackets->inLocation = false;
			if (brackets->inServer)
				brackets->inServer = false;
			return ;
		}
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
	while (1)
	{
		std::getline(configFile, line);
		if (line == "\0")
			break;
		if (line.empty())
			throw(std::invalid_argument("Error, empty line in the file"));

		addLine(line, &configStruct, &brackets, cLine, dispatchTable);
		cLine++;
	}
	configFile.close();
	checkEmptyElem(&configStruct);
	if (brackets.inServer)
		throw(std::invalid_argument("Error, missing closing bracket '}' at the end of file"));
	return (configStruct);

}

int main(void)
{
	std::string test = "                 je suis un test";
	std::string result = extractDirective(test);

	std::cout << "test extract line: " << result << std::endl;
}
