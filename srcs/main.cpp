#include <iostream>
#include "../includes/colors.hpp"
#include "../includes/Config.hpp"
#include "../includes/printDebug.hpp"
#include "../includes/includes.hpp"


int main (void) 
{
	try 
	{
		Config configTest("../../good.conf");
		printConfigClass(configTest);
	} catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	return 0;
}
