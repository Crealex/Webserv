#include "../../includes/socket/includeSend.hpp"

static bool	isServerSocket(int fd, std::vector<Socket *> sockets, std::string &hostnameOfSrvSock)
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
			{
				hostnameOfSrvSock = sockets[i]->getHostname();
				return (true);
			}
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


static int	acceptClient(int fd, std::vector<Client> &clients, Epoll &epoll, std::string hostnameOfSrvSock)
{
	int 		fdClient;
	Client		newClient;
	sockaddr_in	newSockadd;
	socklen_t	addrlen;
	
	fdClient = -1;
	addrlen = sizeof(newSockadd);
	fdClient = accept(fd, (sockaddr *)&newSockadd, &addrlen);
	if (fdClient == -1)
		return (-1);
	sockOptNonBlocking(fdClient);
	newClient.setFdClient(fdClient);
	newClient.setSockadd(newSockadd);
	newClient.setHostname(hostnameOfSrvSock);
	clients.push_back(newClient);
	std::vector<Client>::iterator it = clients.end();
	it--;
	addEpollFd(newClient.getFdClient(), epoll.getNbSockets(), epoll, EPOLLIN|EPOLLOUT);
	return (0);
}

static void	receiveRequest(Client &client)
{
	int		sizeRecv;
	char	buffer[10000];

	sizeRecv = -1;
	sizeRecv = recv(client.getFdClient(), buffer, sizeof(buffer), 0);
	if (sizeRecv == -1)
	{
		return ;
	}
	if (sizeRecv < 10000)
	{
		client.setEndOfFile(true);
	}
	client.setBuf(buffer);
}

static Server	goodServer(Client client, std::vector<Server> servers)
{
	int sizeSrv;
	int	goodIndex;

	sizeSrv = servers.size();
	goodIndex = 0;
	for (int i = 0; i < sizeSrv; i++)
	{
		if (client.getHostname() == servers[i].getHostname())
			goodIndex = i;
	}
	return (servers[goodIndex]);
}

static void	closeClient(std::vector<Client> &clients, int idClient, Epoll &epoll)
{
	int	indexFdEPoll;
	int	sizeEpollEvents;

	indexFdEPoll = 0;
	std::cout << "nb socket epoll before : " << epoll.getNbSockets() << std::endl;
	sizeEpollEvents = epoll.getNbSockets();
	for (int i = 0; i < sizeEpollEvents; i++)
	{
		if (epoll.getEvents()->data.fd == clients[idClient].getFdClient())
		{
			indexFdEPoll = i;
			break ;
		}
	}
	epoll_ctl(epoll.getEpollFd(), EPOLL_CTL_DEL, epoll.getEvents()[indexFdEPoll].data.fd, &epoll.getEvents()[indexFdEPoll]);
	epoll.setNbSockets(sizeEpollEvents - 1);
	std::cout << "nb socket epoll after : " << epoll.getNbSockets() << std::endl;

	std::cout << "nb client before : " << clients.size() << std::endl;
	if (clients[idClient].getFdClient() > -1)
		close(clients[idClient].getFdClient());
	clients.erase(clients.begin() + idClient);
	std::cout << "nb client after : " << clients.size() << std::endl;
}

void	handleClient(std::vector<Socket *> &sockets, std::vector<Server> servers, Epoll &epoll, std::vector<Client> &clients)
{
	int					epollCounterWait;
	int					idClient;
	std::string			hostnameOfSrvSock;
	epoll_event			events[epoll.getNbSockets()];

	epollCounterWait = 0;
	idClient = 0;

	epollCounterWait = epoll_wait(epoll.getEpollFd(), events, epoll.getNbSockets(), 2000);
	if (epollCounterWait < 1)
		return ;
	for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
	{
		if (events[indexEvent].events == EPOLLIN && isServerSocket(events[indexEvent].data.fd, sockets, hostnameOfSrvSock))
		{
			if (acceptClient(events[indexEvent].data.fd, clients, epoll, hostnameOfSrvSock) < 0)
				continue ;
		}
		if (events[indexEvent].events == (EPOLLIN|EPOLLOUT) && isClientSocket(events[indexEvent].data.fd, clients, idClient))
		{
			//std::cout << "receive" << std::endl;
			receiveRequest(clients[idClient]);
			//std::cout << "Reuqest before recieveRequest: " << clients[idClient].getBuf() << std::endl;
		}
		else if (events[indexEvent].events == EPOLLOUT && clients[idClient].getEndOfFile())
		{
			std::cout << "chez kilian" << std::endl;
			sendResponse(clients[idClient], goodServer(clients[idClient], servers));
			if (!clients[idClient].getKeepAlive())
				closeClient(clients, idClient, epoll);
		}
	}
}
