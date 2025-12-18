#include "../../includes/socket/includeSend.hpp"
#include <sys/epoll.h>

static int	countNbEpoll(std::vector<Socket *> &sockets)
{
	int	count;
	int	sizeSockets;

	count = 0;
	sizeSockets = sockets.size();
	for (int i = 0; i < sizeSockets; i++)
	{
		count += sockets[i]->getSockData().size();
		std::cout << "count : " << count << std::endl;
	}
	return (count);
}

static epoll_event	*settingEpollFdServer(std::vector<Socket *> &sockets, int sizeRes, int *epollFds)
{
	size_t		sizeSocket;
	size_t		sizeSockData;
	epoll_event	*res;
	int			indexEpoll;

	res = NULL;
	sizeSocket = sockets.size();
	indexEpoll = 0;
	std::cout << "hereee ? " << std::endl;

	for (size_t i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		std::cout << "hereee 1? " << std::endl;
		for (size_t j = 0; j < sizeSockData; j++)
		{
			epollFds[indexEpoll] = 
			if (tempEpollFds[sizeRes] == -1)
			{

				std::cout << "here ? " << std::endl;
				if (tempEpollFds)
					delete[] tempEpollFds;
				if (*epollFds)
					delete[] *epollFds;
				if (res)
					delete[] res;
				sizeRes = 0;
				return (settingEpollFdServer(sockets, sizeRes, epollFds));
			}
			if (*epollFds)
				delete[] *epollFds;
			*epollFds = tempEpollFds;
			temp = new epoll_event[sizeRes + 1];
			for (int i = 0; i < sizeRes; i++)
				temp[i] = res[i];
			temp[sizeRes].data.fd = sockets[i]->getSockData()[j]->getFdServer();
			temp[sizeRes].events = EPOLLOUT;
			if (epoll_ctl(*epollFds[sizeRes], EPOLL_CTL_ADD, temp[sizeRes].data.fd, &temp[sizeRes]) < 0)
			{
				if (res)
					delete[] res;
				if (*epollFds)
					delete[] *epollFds;
				if (tempEpollFds)
					delete[] tempEpollFds;
				if (temp)
					delete[] temp;
				sizeRes = 0;
				std::cout << "help" << std::endl;
				return (settingEpollFdServer(sockets, sizeRes, epollFds));
			}
			if (res)
				delete[] res;
			res = temp;
			sizeRes++;
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

static void	setEpollClient(int clientFd, epoll_event **fds, int &nbPollFd, int **epollFds)
{
	int			*tempEpollFd;
	epoll_event	*temp;

	temp = new epoll_event[nbPollFd + 1];
	for (int i = 0; i < nbPollFd; i++)
		temp[i] = (*fds)[i];

	tempEpollFd = new int[nbPollFd + 1];
	for (int i = 0; i < nbPollFd; i++)
		tempEpollFd[i] = (*epollFds)[i];
	tempEpollFd[nbPollFd] = epoll_create(1);
	if (tempEpollFd[nbPollFd] == -1)
	{
		if (tempEpollFd)
			delete[] tempEpollFd;
		if (*epollFds)
			delete[] *epollFds;
		if (temp)
			delete[] temp;
		setEpollClient(clientFd, fds, nbPollFd, epollFds);
	}
	temp[nbPollFd].data.fd = clientFd;
	temp[nbPollFd].events = EPOLLIN;
	if (epoll_ctl((*epollFds)[nbPollFd], EPOLL_CTL_ADD, temp[nbPollFd].data.fd, &temp[nbPollFd]) < 0)
	{
		if (temp)
			delete[] temp;
		setEpollClient(clientFd, fds, nbPollFd, epollFds);
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

void	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	size_t		sizeSockets;
	size_t		sizeSocketData;
	char		*bufRecv = new char [conf.getMaxSize()];
	int			*epollFds;
	epoll_event	*fds;
	int			nbEpoll;

	nbEpoll = countNbEpoll(sockets);
	epollFds = new int[nbEpoll];
	fds = new epoll_event[nbEpoll];
	std::cout << "here ? : " << nbEpoll << std::endl;
	fds = settingEpollFdServer(sockets, nbEpoll, epollFds);

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
			setEpollClient(sockets[i]->getSockData()[j]->getFdClient(), &fds, nbEpoll, &epollFds);
			std::cout << "Request: " << std::endl;
			bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
			std::cout << "Request: " << bufRecv << std::endl;
			if (bufRecv)
				sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf.getMaxSize());
		}
	}
}
