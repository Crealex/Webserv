#include "../includes/includes.hpp"
#include "../includes/Socket.hpp"

int	receiveRequest(Config &conf, std::vector<Socket *> &sockets)
{
	size_t	sizeRecv;

	sizeRecv = recv()
	return (0);
}

int	acceptClient(std::vector<Socket *> &sockets, size_t i, size_t j)
{
	int fdClient;

	fdClient = 0;
	fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), nullptr, nullptr);
	if (fdClient < 0)
		return (-1);
	sockets[i]->setFdClient(fdClient, j);
	return (0);
}

int	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t	sizeSockets;
	size_t	sizeSocketData;

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			if (acceptClient(sockets, i, j) < 0)
				continue;
			
		}
	}
	return (0);
}