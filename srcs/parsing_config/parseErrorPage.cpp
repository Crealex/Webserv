#include "../../includes/parseUtils.hpp"

// static void	checkDuplicateCode(std::map<unsigned int, std::string> globalRes, unsigned int toCheck, std::string actualPath, bool &duplicateOk)
// {
// 	unsigned int	nbCodeInVec;
// 	unsigned int	sizeData;

// 	sizeData = globalRes.size();
// 	for (unsigned int i = 0; i < sizeData; i++)
// 	{
// 		nbCodeInVec = globalRes[i].code.size();
// 		for (unsigned int j = 0; j < nbCodeInVec; j++)
// 		{
// 			if (toCheck == globalRes[i].code[j])
// 			{
// 				if (globalRes[i].path.compare(actualPath) != 0)
// 					throw std::invalid_argument(RED "Error : code with multiple error page" RESET);
// 				else
// 					duplicateOk = true;
// 			}
// 		}
// 	}
// }

// std::vector<unsigned int>	getCode(std::vector<std::string> data)
// {
// 	unsigned int				sizeData;
// 	unsigned int				temp;
// 	std::vector<unsigned int>	result;

// 	sizeData = data.size();
// 	for (unsigned int i = 1; i < sizeData - 1; i++)
// 	{
// 		temp = checkDigitValue(data[i], false);
// 		if (temp < 100 || temp > 599)
// 			throw std::invalid_argument(RED "Error : invalid code error" RESET);
// 	}
// 	return (result);
// }

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
