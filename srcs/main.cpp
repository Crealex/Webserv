#include "../includes/colors.hpp"
#include "../includes/Config.hpp"
#include "../includes/printDebug.hpp"
#include "../includes/includes.hpp"

int main (int argc, char **argv) 
{
	if (argc != 2)
	{
		std::cerr << RED << "Invalid number of arguments, you need the executable and the config file" << RESET << std::endl;
		return (-1);
	}
	try 
	{
		Config configTest(argv[1]);
		printConfigClass(configTest);
	} catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	return 0;
}
