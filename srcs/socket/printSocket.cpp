#include "../../includes/includes.hpp"
#include "../../includes/Config.hpp"
#include "../../includes/socket/Socket.hpp"
#include <arpa/inet.h>
#include <cstdio>

void	printSocketListen(std::vector<Socket *> sockets)
{
	if (sockets.size() == 0)
		return ;
	for (std::vector<Socket *>::iterator it1 = sockets.begin(); it1 != sockets.end(); it1++)
	{
		for (std::vector<SocketData *>::iterator it2 = (*it1)->getBegin(); it2 != (*it1)->getEnd(); it2++)
		{
			char	str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &((*it2)->getSockadd().sin_addr), str, INET_ADDRSTRLEN);
			std::cout << "Listening on " << str << ":" << ntohs((*it2)->getSockadd().sin_port) << std::endl;
		}
	}
}
