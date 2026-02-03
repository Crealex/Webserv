#include "../includes/includes.hpp"
#include <cstdlib>

std::time_t	getTimeNow()
{
	time_t	timestamp;
	
	std::time(&timestamp);
	return (timestamp);
}
