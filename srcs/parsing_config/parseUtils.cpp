#include "../../includes/parseUtils.hpp"

/**
 * @brief verify if there's only one semicolon at the end and remove it
 * 
 * @param str string with value
 * @return std::string string value without the semicolon
 */
std::string	removeSemicolon(std::string str)
{
	size_t		indexSemicolon;
	std::string	result;

	indexSemicolon = str.rfind(';');
	if (indexSemicolon == std::string::npos || indexSemicolon != str.size() - 1 || indexSemicolon != str.find(';'))
		throw std::invalid_argument(RED "Error : missing or multiple semicolon" RESET);
	result = str.substr(0, str.size() - 1);
	return (result);
}
/**
 * @brief Get the elt of data in a vector of string
 * 
 * @param data string with the name and the values
 * @return std::vector<std::string> value
 */
std::vector<std::string>	getValue(std::string data)
{
	std::string					temp;
	std::stringstream			ss(data);
	std::vector<std::string>	infos;

	while (ss >> temp)
		infos.push_back(temp);
	return (infos);
}

/**
 * @brief check if the value is not higher than UNIT_MAX and lower than 0
 * 
 * @param str string with the value (number)
 * @param isMaxSize boolean to know if it is used for maxSize of for another value
 * @return unsigned int return the value (number) 
 */
unsigned int	checkDigitValue(std::string str, bool isMaxSize)
{
	unsigned int		res;
	size_t				firstDigitNotZero;
	std::string			allDigitsNotZero;
	std::string			temp;
	std::stringstream	ss;

	allDigitsNotZero = "123456789";
	if (str.find('-') != std::string::npos)
		throw std::invalid_argument(RED "Error : invalid digital value, value is negative" RESET);
	firstDigitNotZero = str.find_first_of(allDigitsNotZero.c_str(), 0);
	if (firstDigitNotZero == std::string::npos)
		return (0);
	else if (firstDigitNotZero > 0)
		temp = str.substr(firstDigitNotZero, str.size() - firstDigitNotZero);
	else
		temp = str;
	if (temp.size() > 10 || (temp.size() == 10 && temp.compare("4294967295") > 0))
	{
		temp = "4294967295";
		if (isMaxSize)
			std::cout << GREEN << "Max size value too high : max size set to 4294967295" << RESET << std::endl;
	}
	ss << temp;
	ss >> res;
	return (res);
}
