#include "../../includes/socket/includeSend.hpp"
#include <cstring>

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

static bool	isClientSocket(int fd, std::vector<Client *> clients, int &idClient)
{
	int	sizeClients;

	sizeClients = clients.size();
	for (int i = 0; i < sizeClients; i++)
	{
		if (fd == clients[i]->getFdClient())
		{
			idClient = i;
			return (true);
		}
	}
	return (false);
}


static int	acceptClient(int fd, std::vector<Client *> &clients, Epoll &epoll, std::string hostnameOfSrvSock)
{
	int 		fdClient;
	Client		*newClient = new Client();
	sockaddr_in	newSockadd;
	socklen_t	addrlen;
	
	fdClient = -1;
	addrlen = sizeof(newSockadd);
	fdClient = ::accept(fd, (sockaddr *)&newSockadd, &addrlen);
	if (fdClient == -1)
		return (-1);
	sockOptNonBlocking(fdClient);
	newClient->setFdClient(fdClient);
	newClient->setSockadd(newSockadd);
	newClient->setHostname(hostnameOfSrvSock);
	clients.push_back(newClient);
	epoll.addEpollFd(newClient->getFdClient(), EPOLLIN);
	return (0);
}

static int	receiveRequest(Client *client, Epoll &epoll)
{
	int		sizeRecv;
	char	buffer[10000];

	sizeRecv = -1;
	sizeRecv = ::recv(client->getFdClient(), buffer, sizeof(buffer) - 1, 0);
	if (sizeRecv == -1)
		return (receiveRequest(client, epoll));
	buffer[sizeRecv] = '\0';
	std::cout << "sizeRecv : " << sizeRecv << std::endl;
	if (sizeRecv == 0)
		return (0);
	client->setBuf(buffer, sizeRecv);
	if (client->getBuf().find("\r\n\r\n") != std::string::npos)
	{
		// client->setEndOfFile(true);
		epoll.setEvents(client, EPOLLOUT);
	}
	client->setTimeoutRequest();
	client->setTimeout();
	return (1);
}

static Server	goodServer(Client *client, std::vector<Server> servers)
{
	int sizeSrv;
	int	goodIndex;

	sizeSrv = servers.size();
	goodIndex = 0;
	for (int i = 0; i < sizeSrv; i++)
	{
		if (client->getHostname() == servers[i].getHostname())
			goodIndex = i;
	}
	return (servers[goodIndex]);
}

void	handleClient(std::vector<Socket *> &sockets, std::vector<Server> servers, Epoll &epoll, std::vector<Client *> &clients)
{
	while (1)
	{
		int					epollCounterWait;
		int					idClient;
		std::string			hostnameOfSrvSock;
		epoll_event			events[epoll.getNbSockets()];

		epollCounterWait = 0;
		idClient = 0;
	
		epollCounterWait = ::epoll_wait(epoll.getEpollFd(), events, epoll.getNbSockets(), 2000);
		std::cout << GREEN << "dana : " << epollCounterWait << std::endl << RESET;
		checkAllTimeout(clients, epoll);
		if (epollCounterWait < 1)
			continue ;
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && isServerSocket(events[indexEvent].data.fd, sockets, hostnameOfSrvSock))
			{
				std::cout << "Just before accept : " << epollCounterWait << std::endl;
			
				if (acceptClient(events[indexEvent].data.fd, clients, epoll, hostnameOfSrvSock) < 0)
					continue ;
				for (size_t i = 0; i < clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << clients[i]->getFdClient() << ", " << clients[i]->getHostname() << ", keep alive : " << clients[i]->getKeepAlive() << std::endl << RESET;
				}
				std::cout << "epoll size : " << epoll.getNbSockets() << std::endl;
			}
			std::cout << GREEN << "epollin : " << events[indexEvent].events << ", " << (isClientSocket(events[indexEvent].data.fd, clients, idClient)) << std::endl << RESET;
			if (events[indexEvent].events & EPOLLIN && isClientSocket(events[indexEvent].data.fd, clients, idClient))
			{
				std::cout << "receive" << std::endl;
				if (receiveRequest(clients[idClient], epoll) == 0)
				{
					std::cout << LIGHT_YELLOW << "Request : " << clients[idClient]->getBuf() << std::endl << RESET;
					closeClient(clients, idClient, epoll);
					continue ;
				}
				std::cout << MAGENTA <<"Request : " << clients[idClient]->getBuf() << std::endl << RESET;
			}
			// std::cout << "event : " << events[indexEvent].data.fd << ", " << events[indexEvent].events << ", " << isClientSocket(events[indexEvent].data.fd, clients, idClient) << std::endl;
			clients[idClient]->checkTimeoutRequest();
			if (events[indexEvent].events & EPOLLOUT && isClientSocket(events[indexEvent].data.fd, clients, idClient))
			{
				std::cout << "chez kilian" << std::endl;
				std::cout << RED << "Request : " << clients[idClient]->getBuf() << std::endl << RESET;
				std::cout << "maybe after" << std::endl;
				sendResponse(clients[idClient], goodServer(clients[idClient], servers));
				// std::cout << BLUE << "after send : " << clients[idClient]->getKeepAlive() << std::endl << RESET;
				for (size_t i = 0; i < clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << clients[i]->getFdClient() << ", " << clients[i]->getHostname() << ", keep alive : " << clients[i]->getKeepAlive() << std::endl << RESET;
				}
				if (clients[idClient]->getKeepAlive() == false)
					closeClient(clients, idClient, epoll);
				else
				{
					clients[idClient]->resetClient();
					epoll.setEvents(clients[idClient], EPOLLIN);
				}
				std::cout << "end" << std::endl;
			}
		}
	}
}
