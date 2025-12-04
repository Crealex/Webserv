#include "../includes/includes.hpp"
#include "../includes/Config.hpp"
#include "../includes/Socket.hpp"
#include <arpa/inet.h>

void	printSocketListen(std::vector<Socket> sockets)
{
	if (sockets.size() == 0)
		return ;
	for (std::vector<Socket>::iterator it1 = sockets.begin(); it1 != sockets.end(); it1++)
	{
		for (std::vector<SocketData>::iterator it2 = it1->getSockData().begin(); it2 != it1->getSockData().end(); it2++)
		{
			char	str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, (struct sockaddr_in *)&(it2->getSockadd().sin_addr), str, INET_ADDRSTRLEN);
			std::cout << "Listening on " << str << ":" << ntohs(it2->getSockadd().sin_port) << std::endl;
		}
	}
}

void	createSocket(Config conf)
{
	std::vector<Socket>	sockets;
	size_t				sizeAddPort;
	int					checkFail;

	sizeAddPort = conf.getAddressPort().size();
	sockets.reserve(sizeAddPort);
	for (size_t i = 0; i < sizeAddPort; i++)
	{
		Socket	temp(conf.getAddressPort()[i]);
		sockets.push_back(temp);
	}
	if (sockets.size() == 0)
		createSocket(conf);
	for (std::vector<Socket>::iterator itSock = sockets.begin(); itSock != sockets.end(); itSock++)
	{
		for (std::vector<SocketData>::iterator itSD = itSock->getSockData().begin(); itSD != itSock->getSockData().end(); itSD++)
		{
			checkFail = bind(itSD->getFdServer(), (struct sockaddr *)&(itSD->getSockadd()), sizeof(sockaddr_in));
			if (checkFail < 0)
				itSock->getSockData().erase(itSD);
			checkFail = listen(itSD->getFdServer(), 2);
			if (checkFail < 0)
				itSock->getSockData().erase(itSD);
			
		}
	}
}