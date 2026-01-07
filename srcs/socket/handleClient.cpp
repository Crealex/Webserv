#include "../../includes/socket/includeSend.hpp"
#include "../../includes/epoll/Epoll.hpp"
#include "../../includes/Client.hpp"

static bool	isServerSocket(int fd, std::vector<Socket *> sockets)
{
	int	sizeSockets;
	int	nbSockData;

	sizeSockets = sockets.size();
	for (int i = 0; i < sizeSockets; i++)
	{
		nbSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < nbSockData; j++)
		{
			if (fd == sockets[i]->getSockData()[j]->getFdServer())
				return (true);
		}
	}
	return (false);
}

static bool	isClientSocket(int fd, std::vector<Socket *> sockets)
{
	int	sizeSockets;
	int	nbSockData;

	sizeSockets = sockets.size();
	for (int i = 0; i < sizeSockets; i++)
	{
		nbSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < nbSockData; j++)
		{
			if (fd == sockets[i]->getSockData()[j]->getFdClient())
				return (true);
		}
	}
	return (false);
}


static int	acceptClient(int fd, std::vector<Client> &clients, Epoll &epoll)
{
	int 		fdClient;
	Client		newClient;
	sockaddr_in	newSockadd;
	socklen_t addrlen;
	
	
	fdClient = -1;
	addrlen = sizeof(newSockadd);
	fdClient = accept(fd, (sockaddr *)&newSockadd, &addrlen);
	if (fdClient == -1)
		return (-1);
	sockets[i]->setFdClient(fdClient, j);
	newClient.setFdClient(sockets[i]->getSockData()[j]->getFdClient());
	newClient.setSockadd(newSockadd);
	clients.push_back(newClient);
	addEpollFd(sockets[i]->getSockData()[j]->getFdClient(), epoll.getNbSockets(), epoll);
	return (0);
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
	int					epollCounterWait;
	size_t				sizeSockets;
	size_t				sizeSocketData;
	std::vector<Client>	clients;
	epoll_event			events[epoll.getNbSockets()];

	epollCounterWait = 0;

	sizeSockets = sockets.size();
	epollCounterWait = epoll_wait(epoll.getEpollFd(), events, epoll.getNbSockets(), 2000);
	if (epollCounterWait < 1)
		return ;
	for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
	{
		if (events[indexEvent].events == EPOLLIN && isServerSocket(events[indexEvent].data.fd, sockets))
		{
			if (acceptClient(events[indexEvent], clients, epoll) < 0)
				continue ;
		}
		else if (isClientSocket(events[indexEvent].data.fd, sockets))
		{
			if (events[i].events == EPOLLIN)
			{
				bufRecv = receiveRequest(conf, sockets[i]->getSockData()[j]->getFdClient(), bufRecv);
			}
			else
			{
				if (bufRecv)
				{
					sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf.getMaxSize());
				}
			}
		}
	}
}
