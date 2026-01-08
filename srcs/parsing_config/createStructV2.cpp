#include "../../includes/configStruct.hpp"

server CreateStruct(std::string configPath)
{
	server configStruct;
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
