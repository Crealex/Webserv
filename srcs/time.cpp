#include "../includes/includes.hpp"
#include "../includes/socket/includeSend.hpp"
#include <cstdlib>

std::time_t	getTimeNow()
{
	time_t	timestamp;
	
	std::time(&timestamp);
	return (timestamp);
}
