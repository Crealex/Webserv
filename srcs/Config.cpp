
#include "../includes/Config.hpp"
#include "../includes/colors.hpp"

Config::Config()
{
	std::cout << GREEN << "Default Config constructor called" << RESET << std::endl;
}

Config::Config(const Config &cpy)
{
	std::cout << BLUE << "Copy constructor called" << RESET << std::endl;
}

Config::~Config()
{
	std::cout << RED << "Destructor called" << RESET << std::endl;
}

Config &Config::operator=(const Config &rhs)
{
	return (*this);
}
