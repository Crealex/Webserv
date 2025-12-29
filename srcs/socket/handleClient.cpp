#include "../../includes/socket/includeSend.hpp"
#include "../../includes/epoll/Epoll.hpp"

static int	acceptClient(std::vector<Socket *> &sockets, size_t i, size_t j)
{
	int fdClient;

	fdClient = -1;
	fdClient = accept(sockets[i]->getSockData()[j]->getFdServer(), NULL, NULL);
	if (fdClient == -1)
	{
		std::cerr << "Error, with accept" << std::endl;
		return (-1);
	}
	sockets[i]->setFdClient(fdClient, j);
	return (0);
}

static void	addEpollClient(int clientFd, int nbPollFd, Epoll epoll)
{
	epoll_event	*temp;

	temp = new epoll_event[nbPollFd + 1];
	for (int i = 0; i < nbPollFd; i++)
		temp[i] = epoll.getEvents()[i];

	temp[nbPollFd].data.fd = clientFd;
	temp[nbPollFd].events = EPOLLIN;
	if (epoll_ctl(epoll.getEpollFd(), EPOLL_CTL_ADD, temp[nbPollFd].data.fd, &temp[nbPollFd]) < 0)
	{
		if (temp)
			delete[] temp;
		addEpollClient(clientFd, nbPollFd, epoll);
	}
	epoll.setEvents(temp);
	epoll.setNbSockets(nbPollFd + 1);
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

void	handleClient(std::vector<Socket *> &sockets, Config conf, Epoll &epoll)
{
	size_t		sizeSockets;
	size_t		sizeSocketData;
	char		*bufRecv = new char [conf.getMaxSize()];
	int			epollCounterWait;

	epollCounterWait = 0;

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			if (acceptClient(sockets, i, j) < 0)
				continue ;
			addEpollClient(sockets[i]->getSockData()[j]->getFdClient(), epoll.getNbSockets(), epoll);
			epollCounterWait = epoll_wait(epoll.getEpollFd(), epoll.getEvents(), epoll.getNbSockets(), 2000);
			if (epollCounterWait < 1)
				continue ;
			for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
			{
				std::cout << "Request: " << std::endl;
				bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
				std::cout << "Request: " << bufRecv << std::endl;
				if (bufRecv)
				{
					std::cout << "before send ? " << std::endl;
					sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf.getMaxSize());
					std::cout << "after send ? " << std::endl;
				}
			}
		}
	}
}
