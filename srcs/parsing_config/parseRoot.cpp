#include "../../includes/parseUtils.hpp"

std::string parseRoot(std::string data)
{
	std::vector<std::string>    infos;
	int							isAccessible;

	infos = getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple root path" RESET);
	isAccessible = access(infos[1].c_str(), F_OK & R_OK);
	if (isAccessible == -1)
		throw std::invalid_argument(RED "Error : invalid root path" RESET);
	return (infos[1]);
}