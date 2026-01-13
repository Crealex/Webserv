#include "../../includes/parseUtils.hpp"

std::string parseRoot(std::string data)
{
	std::vector<std::string>    infos;
	std::fstream				file;

	infos = getValue(removeSemicolon(data));
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple root path" RESET);
	file.open(infos[1].c_str(), std::ios::in);
	if (!file.is_open())
		throw std::invalid_argument(RED "Error : invalid root path" RESET);
	file.close();
	return (infos[1]);
}