#include "../includes/colors.hpp"
#include "../includes/Config.hpp"
#include "../includes/printDebug.hpp"
#include "../includes/includes.hpp"
#include "../includes/socket/includeSocket.hpp"
#include "../includes/socket/includeClient.hpp"


int main (int argc, char **argv) 
{
	if (argc > 2)
	{
		std::cerr << RED << "Invalid number of arguments, you need the executable and maybe a config file" << RESET << std::endl;
		return (-1);
	}
	try 
	{
		Config configTest(argv[1]);
		printConfigClass(configTest);
		if (createSocket(configTest) < 0)
		{
			std::cerr << RED << "Error : no socket for the webserv" << std::endl << RESET;
			return (-2);
		}
	}
	catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	return 0;
}
