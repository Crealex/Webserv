#include "../../includes/parseUtils.hpp"

std::string	parseHostname(std::string data)
{
	std::vector<std::string>	infos;

	infos = getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple hostname" RESET);
	return (infos[1]);
}
