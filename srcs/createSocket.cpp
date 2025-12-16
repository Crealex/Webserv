#include "../includes/includes.hpp"
#include "../includes/Config.hpp"
#include "../includes/Socket.hpp"
#include <arpa/inet.h>
#include <cstdio>

static void	printSocketListen(std::vector<Socket *> sockets)
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

static void	listenSocket(std::vector<Socket *> &sockets, int i, int j)
{
	int									checkFail;
	std::vector<SocketData *>::iterator	eltToErase;

	checkFail = 0;
	checkFail = listen(sockets[i]->getSockData()[j]->getFdServer(), 2);
	if (checkFail < 0)
		sockets[i]->eraseSocket(j);
}

static int	bindSocket(std::vector<Socket *> &sockets)
{
	int		checkFail;
	size_t	sizeSockets;
	size_t	sizeSockData;

	checkFail = 0;
	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (size_t j = 0; j != sizeSockData; j++)
		{
			checkFail = bind(sockets[i]->getSockData()[j]->getFdServer(), (struct sockaddr *)&(sockets[i]->getSockData()[j]->getSockadd()), sizeof(sockaddr_in));
			if (checkFail < 0)
				sockets[i]->eraseSocket(j);
			else
				listenSocket(sockets, i, j);
		}
		if (sockets[i]->getSockData().size() == 0)
		{
			sockets.erase(sockets.begin() + i);
			if (sockets.size() == 0)
				return (-1);
		}
	}
	return (0);
}

int	createSocket(Config conf)
{
	std::vector<Socket *>	sockets;
	size_t				sizeAddPort;

	sizeAddPort = conf.getAddressPort().size();
	sockets.reserve(sizeAddPort);
	for (size_t i = 0; i < sizeAddPort; i++)
	{
		Socket	*temp = new Socket(conf.getAddressPort()[i]);
		sockets.push_back(temp);
	}
	if (sockets.size() == 0)
		return (-1);
	if (bindSocket(sockets) < 0)
		return (-1);
	printSocketListen(sockets);
	return (0);
}
