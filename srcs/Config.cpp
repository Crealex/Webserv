
#include "../includes/Config.hpp"
#include "../includes/colors.hpp"

Config::Config(std::string path)
{
	// parsing config file
	std::cout << GREEN << "Default Config constructor called" << RESET << std::endl;
}

Config::~Config()
{
	std::cout << RED << "Destructor called" << RESET << std::endl;
}

std::string Config::getAddress() const
{
	return (this->address);
}
