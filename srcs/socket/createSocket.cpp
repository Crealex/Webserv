#include "../../includes/includes.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/socket/Socket.hpp"
#include <arpa/inet.h>

static int	listenSocket(std::vector<Socket *> &sockets, size_t &i, size_t &j, size_t &sizeSockData)
{
	int									checkFail;
	std::vector<SocketData *>::iterator	eltToErase;

	checkFail = 0;
	checkFail = ::listen(sockets[i]->getSockData()[j]->getFdServer(), 2);
	if (checkFail < 0)
	{
		sockets[i]->eraseSocket(j);
		j--;
		sizeSockData--;
		return (0);
	}
	return (1);
}

static int	bindSocket(std::vector<Socket *> &sockets)
{
	int		checkFail;
	int		nbSockets;
	size_t	sizeSockets;
	size_t	sizeSockData;

	checkFail = 0;
	nbSockets = 0;
	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (size_t j = 0; j != sizeSockData; j++)
		{
			checkFail = ::bind(sockets[i]->getSockData()[j]->getFdServer(), (struct sockaddr *)&(sockets[i]->getSockData()[j]->getSockadd()), sizeof(sockaddr_in));
			if (checkFail < 0)
			{
				sockets[i]->eraseSocket(j);
				j--;
				sizeSockData--;
			}
			else
			{
				nbSockets += listenSocket(sockets, i, j, sizeSockData);
			}
		}
		if (sockets[i]->getSockData().size() == 0)
		{
			sockets.erase(sockets.begin() + i);
			i--;
			sizeSockets--;
		}
	}
	return (nbSockets);
}

std::vector<Socket *>	createSocket(std::vector<Server> srvs, int &nbSockets)
{
	std::vector<Socket *>	sockets;
	size_t					sizeSrvs;

	sizeSrvs = srvs.size();
	sockets.reserve(sizeSrvs);
	for (size_t i = 0; i < sizeSrvs; i++)
	{
		Socket	*temp = new Socket(srvs[i]);
		sockets.push_back(temp);
	}
	nbSockets = bindSocket(sockets);
	return (sockets);
}
