#ifndef PARSEUTILS_HPP

#define PARSEUTILS_HPP

#include "includes.hpp"
#include "Config.hpp"
#include <sstream>
#include <fstream>

std::string					removeSemicolon(std::string str);
std::vector<std::string>	getValue(std::string data);
unsigned int				checkDigitValue(std::string str, bool isMaxSize);

#endif