#include "../../includes/parseUtils.hpp"

std::map<unsigned int, std::string>	parseErrorPage(std::vector<std::string> data)
{
	std::map<unsigned int, std::string>		result;
	std::vector<std::string>				infos;
	unsigned int							code;
	std::string								pathError;
	unsigned int							nbError;
	std::fstream							file;

	nbError = data.size();
	for (unsigned int i = 0; i < nbError; i++)
	{
		infos = getValue(removeSemicolon(data[i]));
		if (infos.size() != 3)
			throw std::invalid_argument(RED "Error : missing path / code or multiple path / code for error pages" RESET);
		code = checkDigitValue(infos[1], false);
		if (code < 100 || code > 599)
			throw std::invalid_argument(RED "Error : invalid code error" RESET);
		pathError = infos[2];
		file.open(pathError.c_str(), std::ios::in);
		if (!file.is_open())
			throw std::invalid_argument(RED "Error : invalid path for error pages" RESET);
		file.close();
		result.insert({code, pathError});
	}
	return (result);
}
