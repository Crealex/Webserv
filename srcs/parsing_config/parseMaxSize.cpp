#include "../../includes/parseUtils.hpp"

static void	checkMetricPrefix(std::vector<std::string> &infos)
{
	std::string	metricPrefix;
	size_t		firstNotPrefix;

	metricPrefix = "kKmMgGtT";
	firstNotPrefix = infos[2].find_first_not_of(metricPrefix.c_str(), 0);
	if (firstNotPrefix != std::string::npos)
		throw std::invalid_argument(RED "Error : invalid max size, invalid metric prefix" RESET);
	if (infos[2] == "k" || infos[2] == "K")
		infos[1].append("000");
	if (infos[2] == "m" || infos[2] == "M")
		infos[1].append("000000");
	if (infos[2] == "g" || infos[2] == "G")
		infos[1].append("000000000");
}

unsigned int	parseMaxSize(std::string data)
{
	std::vector<std::string>	infos;
	int							sizeInfos;
	std::string					temp;
	unsigned int				result;

	infos = getValue(data);
	sizeInfos = infos.size();
	if (sizeInfos < 2 || sizeInfos > 3)
		throw std::invalid_argument(RED "Error : invalid max size" RESET);
	if (sizeInfos == 3)
		checkMetricPrefix(infos);
	result = checkDigitValue(infos[1], true);
	return (result);
}