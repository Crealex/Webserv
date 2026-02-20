#include "../../includes/configStruct.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/printDebug.hpp"

struct bracketData {
	bool inServer;
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
	while (!line->empty() && (line->at(0) == '	' || line->at(0) == ' '))
	{
		line->erase(0, 1);
	}
}

static void updateLocationBracket(std::string &line, server *srv, bracketData *brackets)
{
	unsigned int locI;

	if (brackets->inLocation == true)
		throw std::invalid_argument("Error, missing closing bracket for the previous location");
	brackets->inLocation = true;
	srv->locations.push_back(location());
	locI = srv->locations.size() - 1;
	srv->locations.at(locI).path = line;
}

static void addElem(std::string &line, server *srv, bracketData *brackets)
{
	std::string directive = extractDirective(line);
	unsigned int LocI = srv->locations.size() - 1;

	if (brackets->inLocation)
	{
		if (directive == "autoIndex")
			srv->locations[LocI].autoIndex = line;
		else if (directive == "method")
			srv->locations[LocI].allowedMethods = line;
		else if (directive == "index")
			srv->locations[LocI].index = line;
		else if (directive == "return")
			srv->locations[LocI].ret = line;
		else if (directive == "uploadPath")
			srv->locations[LocI].uploadPath = line;
		else
			throw std::invalid_argument("Error with the line: " + line);
	}
	else if (brackets->inServer)
	{
		if (directive == "hostname")
			srv->hostname = line;
		else if (directive == "root")
			srv->root = line;
		else if (directive == "maxSize")
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
		if (directive == "cgi")
			srv->locations[locI].cgi.push_back(line);
		else
			throw std::invalid_argument("Error, impossible to add this line: " + line);
		
	}
	else if (brackets->inServer)
	{
		if (directive == "listen")
			srv->listen.push_back(line);
		else if (directive == "errorPage")
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

void checkEmptyElem(server *srv)
{
	if (srv->maxSize.empty())
		throw std::invalid_argument("Error, missing maxSize");
	if (srv->locations.size() == 0)
		throw std::invalid_argument("Error, missing locations");
	if (srv->root.empty())
		throw std::invalid_argument("Error, missing root");
	if (srv->listen.size() == 0)
		throw std::invalid_argument("Error, missing listen");
	if (srv->hostname.empty())
		throw std::invalid_argument("Error, missing hostname");
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

server createStruct(std::ifstream *configFile, bool *eof)
{
	server configStruct;
	std::string line;
	bracketData brackets;
	static std::size_t cLine;
	std::map<std::string, directiveHandler> dispatchTable;

	dispatchTable = createDispatchTable();
	cLine = 1;
	brackets.inServer = true;
	while (std::getline(*configFile, line))
	{
		addLine(line, &configStruct, &brackets, cLine, dispatchTable);
		cLine++;
		if (brackets.inServer == false && !configFile->eof())
		{
			return (configStruct);
		}
	}
	configFile->close();
	checkEmptyElem(&configStruct);
	if (brackets.inServer)
		throw(std::invalid_argument("Error, missing closing bracket '}' at the end of file"));
	if (configFile->eof())
		*eof = true;
	return (configStruct);

}

std::vector<server> createVectStructSrv(std::string configPath)
{
	std::vector<server> vectSrv;
	std::ifstream configFile;
	std::string line;
	bool eof;

	configFile.open(configPath.c_str());
	if (!configFile.is_open())
		throw std::invalid_argument("Error, invalid path for the config file");
	while (std::getline(configFile, line))
	{
		if (line == "server {")
			vectSrv.push_back(createStruct(&configFile, &eof));
		else if (line.empty()) {
			continue;
		}
		else
			throw std::invalid_argument("Error, invalid directive, excpected: server { , reality: " + line);
	}
	return (vectSrv);
}


//int main(void)
//{
//	try 
//	{
//		std::vector<server> testStruct = createVectStructSrv("../../newGood.conf");
//		unsigned int i = 0;
//		while (i < testStruct.size())
//		{
//			std::cout << BOLD << MAGENTA << "Server " << i << ":" << RESET << std::endl;
//			printStructV2(testStruct.at(i));
//			i++;
//		}
//	} 
//	catch (std::exception &e) 
//	{
//		std::cout << RED << e.what() << RESET << std::endl;
//	}
//}
