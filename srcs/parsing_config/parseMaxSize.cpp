#include "../../includes/parseUtils.hpp"

unsigned int	parseMaxSize(std::string data)
{
	std::vector<std::string>	infos;
	std::string					temp;
	unsigned int				result;


	infos = getValue(data);
	if (infos.size() != 2)
		throw std::invalid_argument(RED "Error : missing or multiple max size" RESET);
	result = checkDigitValue(infos[1], true);
	return (result);
}