#include "../../includes/socket/includeSend.hpp"
#include <fstream>

static pollfd	*settingPollFds(std::vector<Socket *> &sockets, int &sizeRes)
{
	size_t	sizeSocket;
	size_t	sizeSockData;
	pollfd	*res;
	pollfd	*temp;

	sizeSocket = sockets.size();
	res = NULL;
	temp = NULL;
	for (size_t i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSockData; j++)
		{
			temp = new pollfd[sizeRes + 1];
			for (int i = 0; i < sizeRes; i++)
				temp[i] = res[i];
			temp[sizeRes].fd = sockets[i]->getSockData()[j]->getFdServer();
			temp[sizeRes].events = POLLIN;
			if (res)
				delete[] res;
			res = temp;
			sizeRes++;
		}
	}
	return (res);
}

static void	acceptClient(std::vector<Socket *> &sockets, size_t i, size_t j)
{
	int fdClient;

	fdClient = -1;
	fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), NULL, NULL);
	std::cout << "after accept" << std::endl;
	if (fdClient == -1)
		std::cerr << "Error, with accept" << std::endl;
	sockets[i]->setFdClient(fdClient, j);
}

static char	*receiveRequest(Config &conf, int fdClient, char *bufRecv)
{
	int			sizeRecv;

	sizeRecv = -1;
	sizeRecv = recv(fdClient, bufRecv, conf.getMaxSize() - 1, 0);
	if (sizeRecv == -1)
		std::cerr << "erreur avec recv" << std::endl;
	bufRecv[sizeRecv] = '\0';
	return (bufRecv);
}

void	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t	sizeSockets;
	size_t	sizeSocketData;
	char	*bufRecv = new char [conf.getMaxSize()];
	int		countPollEvent;
	pollfd	*fds;
	int		nbPoll;

	nbPoll = 0;
	fds = settingPollFds(sockets, nbPoll);
	std::cout << "maybe" << std::endl;
	(void)fds;
	(void)countPollEvent;
	// countPollEvent = poll(fds, nbPoll, 2500);

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		std::cout << "maybe" << std::endl;
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			std::cout << "maybe" << std::endl;
			acceptClient(sockets, i, j);
			std::cout << "Request: " << std::endl;
			bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
			std::cout << "Request: " << bufRecv << std::endl;
			sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
		}
	}
}
