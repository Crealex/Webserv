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

static bool	isClientSocket(int fd, std::vector<Client> clients, int &idClient)
{
	int	sizeClients;

	sizeClients = clients.size();
	for (int i = 0; i < sizeClients; i++)
	{
		if (fd == clients[i].getFdClient())
		{
			idClient = i;
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
	// sockets[i]->setFdClient(fdClient, j);
	sockOptNonBlocking(fdClient);
	newClient.setFdClient(fdClient);
	newClient.setSockadd(newSockadd);
	clients.push_back(newClient);
	addEpollFd(newClient.getFdClient(), epoll.getNbSockets(), epoll);
	return (0);
}

static void	receiveRequest(Config &conf, Client client)
{
	int		sizeRecv;
	char	*buffer;

	sizeRecv = -1;
	sizeRecv = recv(client.getFdClient(), buffer, 10000, 0);
	if (sizeRecv == -1 || sizeRecv < 10000)
	{
		client.
	}
	client.setBuf(buffer);
}

void	handleClient(std::vector<Socket *> &sockets, Config conf, Epoll &epoll)
{
	int					epollCounterWait;
	int					idClient;
	size_t				sizeSockets;
	size_t				sizeSocketData;
	std::vector<Client>	clients;
	epoll_event			events[epoll.getNbSockets()];

	epollCounterWait = 0;
	idClient = 0;

	sizeSockets = sockets.size();
	epollCounterWait = epoll_wait(epoll.getEpollFd(), events, epoll.getNbSockets(), 2000);
	if (epollCounterWait < 1)
		return ;
	for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
	{
		if (events[indexEvent].events == EPOLLIN && isServerSocket(events[indexEvent].data.fd, sockets))
		{
			if (acceptClient(events[indexEvent].data.fd, clients, epoll) < 0)
				continue ;
		}
		else if (isClientSocket(events[indexEvent].data.fd, clients, idClient))
		{
			if (events[indexEvent].events == EPOLLIN)
			{
				receiveRequest(conf, clients[idClient]);
			}
			else
			{
				sendResponse(sockets[i]->getSockData()[j]->getFdClient(), bufRecv, conf.getMaxSize());
			}
		}
	}
}
