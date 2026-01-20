#include "../includes/includes.hpp"

std::time_t	getTimeNow()
{
	time_t	timestamp;
	
	time(&timestamp);
	return (timestamp);
}