#include "../../includes/socket/includeSend.hpp"
#include <sys/epoll.h>

static int	createEpoll()
{
	int	res;

	res = epoll_create(1);
	if (res == -1)
	{
		std::cout << "here ? " << std::endl;
		return (createEpoll());
	}
	return (res);
}

static epoll_event	*addEpollServer(std::vector<Socket *> &sockets, int sizeRes, int epollFd)
{
	epoll_event	*res;
	int			sizeSocket;
	int			sizeSockData;
	int			count;

	res = new epoll_event[sizeRes];
	sizeSocket = sockets.size();
	count = 0;
	for (int i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < sizeSockData; j++)
		{
			res[count].data.fd = sockets[i]->getSockData()[j]->getFdServer();
			res[count].events = EPOLLOUT;
			if (epoll_ctl(epollFd, EPOLL_CTL_ADD, res[count].data.fd, &res[count]) < 0)
			{
				if (res)
					delete[] res;
				return (addEpollServer(sockets, sizeRes, epollFd));
			}
		}
	}
	return (res);
}


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

static void	addEpollClient(int clientFd, epoll_event **fds, int &nbPollFd, int epollFd)
{
	epoll_event	*temp;

	temp = new epoll_event[nbPollFd + 1];
	for (int i = 0; i < nbPollFd; i++)
		temp[i] = (*fds)[i];

	temp[nbPollFd].data.fd = clientFd;
	temp[nbPollFd].events = EPOLLIN;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, temp[nbPollFd].data.fd, &temp[nbPollFd]) < 0)
	{
		if (temp)
			delete[] temp;
		addEpollClient(clientFd, fds, nbPollFd, epollFd);
	}
	nbPollFd++;
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

void	handleClient(std::vector<Socket *> &sockets, Config conf, int nbSockets)
{
	size_t		sizeSockets;
	size_t		sizeSocketData;
	char		*bufRecv = new char [conf.getMaxSize()];
	int			epollFd;
	epoll_event	*fdsEvent;
	int			epollCounterWait;

	epollCounterWait = 0;
	epollFd = createEpoll();
	std::cout << "here ? : " << std::endl;
	fdsEvent = addEpollServer(sockets, nbSockets, epollFd);

	sizeSockets = sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		std::cout << "maybe 1" << std::endl;
		sizeSocketData = sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocketData; j++)
		{
			std::cout << "maybe 2" << std::endl;
			if (acceptClient(sockets, i, j) < 0)
				continue ;
			std::cout << "bruh" << std::endl;
			addEpollClient(sockets[i]->getSockData()[j]->getFdClient(), &fdsEvent, nbSockets, epollFd);
			epollCounterWait = epoll_wait(epollFd, fdsEvent, nbSockets, 2000);
			if (epollCounterWait < 1)
				continue ;
			for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
			{
				std::cout << "Request: " << std::endl;
				bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
				std::cout << "Request: " << bufRecv << std::endl;
				if (bufRecv)
					sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf.getMaxSize());
			}
		}
	}
}
