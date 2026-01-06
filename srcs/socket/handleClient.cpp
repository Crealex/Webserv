#include "../../includes/socket/includeSend.hpp"
#include <fstream>

static char *receiveRequest(Config &conf, int fdClient, char *bufRecv)
{
	int			sizeRecv;

	sizeRecv = -1;
	sizeRecv = recv(fdClient, bufRecv, conf.getMaxSize() - 1, 0);
	if (sizeRecv == -1)
		std::cerr << "erreur avec recv" << std::endl;
	bufRecv[sizeRecv] = '\0';
	return (bufRecv);
}

static void	acceptClient(std::vector<Socket *> &sockets, size_t i, size_t j)
{
	int fdClient;

	fdClient = -1;
	fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), NULL, NULL);
	if (fdClient)
		std::cerr << "Error, with accept" << std::endl;
	sockets[i]->setFdClient(fdClient, j);
}

void	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t	sizeSockets;
	size_t	sizeSocketData;
	char *bufRecv = new char [conf.getMaxSize()];

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			acceptClient(sockets, i, j);
			bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
			std::cout << "Request: " << bufRecv << std::endl;
			sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf);
		}
	}
}
