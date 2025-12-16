#include "../includes/includes.hpp"
#include "../includes/Socket.hpp"

std::string	receiveRequest(Config &conf, int fdClient)
{
	int			sizeRecv;
	char		bufRecv[conf.getMaxSize()];
	std::string	res;

	sizeRecv = -1;
	while (sizeRecv == -1 || bufRecv == NULL)
		sizeRecv = recv(fdClient, bufRecv, conf.getMaxSize() - 1, 0);
	bufRecv[conf.getMaxSize()] = '\0';
	res = bufRecv;
	return (res);
}

void	acceptClient(std::vector<Socket *> &sockets, size_t i, size_t j)
{
	int fdClient;

	fdClient = -1;
	while (fdClient == -1)
		fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), NULL, NULL);
	sockets[i]->setFdClient(fdClient, j);
}

int	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t		sizeSockets;
	size_t		sizeSocketData;
	std::string	bufRecv;

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			acceptClient(sockets, i, j);
			bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient());
			(void)bufRecv;
		}
	}
	return (0);
}