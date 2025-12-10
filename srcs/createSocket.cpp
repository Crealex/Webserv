#include "../includes/includes.hpp"
#include "../includes/Config.hpp"
#include "../includes/Socket.hpp"
#include <arpa/inet.h>
#include <cstdio>

static void	printSocketListen(std::vector<Socket> sockets)
{
	if (sockets.size() == 0)
		return ;
	for (std::vector<Socket>::iterator it1 = sockets.begin(); it1 != sockets.end(); it1++)
	{
		for (std::vector<SocketData *>::iterator it2 = it1->getBegin(); it2 != it1->getEnd(); it2++)
		{
			char	str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &((*it2)->getSockadd().sin_addr), str, INET_ADDRSTRLEN);
			std::cout << "Pooooooooooort : " << (*it2)->getSockadd().sin_port << std::endl;
			std::cout << "Listening on " << str << ":" << ntohs((*it2)->getSockadd().sin_port) << std::endl;
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
	checkFail = 0;
	for (size_t i = 0; i < sizeAddPort; i++)
	{
		Socket	temp(conf.getAddressPort()[i]);
		sockets.push_back(temp);
	}
	if (sockets.size() == 0)
		createSocket(conf);
	std::cout << &(sockets[0].getSockData()[0]) << std::endl;
	std::cout << sockets[0].getSockData()[0]->getFdServer() << "     " << sockets.begin()->getSockData()[0]->getFdServer() << std::endl;
	for (std::vector<Socket>::iterator itSock = sockets.begin(); itSock != sockets.end(); itSock++)
	{
		std::cout << &(itSock->getSockData()[0]);
		std::cout << itSock->getSockData()[0]->getSockadd().sin_port << std::endl;
		for (std::vector<SocketData *>::iterator itSD = itSock->getBegin(); itSD != itSock->getEnd(); itSD++)
		{
			if (itSD == itSock->getBegin())
			{
				std::cout << &itSD << std::endl;
				std::cout << itSock->getSockData()[0]->getSockadd().sin_port << std::endl;
				std::cout << (*itSD)->getSockadd().sin_port << std::endl;
				std::cout << itSock->getSockData()[0]->getFdServer() << std::endl;
				std::cout << (*itSD)->getFdServer() << std::endl;
			}
			checkFail = bind((*itSD)->getFdServer(), (struct sockaddr *)&((*itSD)->getSockadd()), sizeof(sockaddr_in));
			std::perror("bind");
			if (checkFail < 0)
				itSock->eraseSocket(itSD);
			checkFail = listen((*itSD)->getFdServer(), 2);
			if (checkFail < 0)
				itSock->eraseSocket(itSD);
			if (itSock->getSockData().size() == 0)
				break ;
		}
	}
	printSocketListen(sockets);
}