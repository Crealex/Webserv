#include "../../includes/parseUtils.hpp"

std::string	parseHostname(std::string data)
{
	std::vector<std::string>	infos;

	std::cout << "data : " << data << std::endl;
	infos = getValue(removeSemicolon(data));
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple hostname" RESET);
	return (infos[1]);
}
