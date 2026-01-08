#include "../../includes/socket/includeSend.hpp"
#include "../../includes/epoll/Epoll.hpp"
#include "../../includes/Client.hpp"

static bool	isServerSocket(int fd, std::vector<Socket *> sockets)
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
	//std::cout << "SocketData:\n FdServer:" << sockets[i]->getSockData()[j]->getFdServer() << std::endl;
	fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), NULL, NULL);
	std::cout << "in acceptCLient" << std::endl;
	if (fdClient == -1)
		std::cerr << "Error, with accept" << std::endl;
	sockets[i]->setFdClient(fdClient, j);
}

void	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t	sizeSockets;
	size_t	sizeSocketData;
	char *bufRecv = new char [conf.getMaxSize()];

	sizeSockets = sockets.size();
	std::cout << BOLD << "in handleClient" << RESET << std::endl;
	for (size_t i = 0; i < sizeSockets; i++)
	{
		nbSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < nbSockData; j++)
		{
			acceptClient(sockets, i, j);
			bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
			std::cout << "Request: " << bufRecv << std::endl;
			sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf);
			close(sockets[i]->getSockData()[j]->getFdClient()); // INFO: Temporaire pour faires mes test (Alex)
		}
		std::cout << BOLD << "out handClient" << RESET << std::endl;
	}
}
