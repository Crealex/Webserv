#include "../includes/Loop.hpp"

// Constructor & Destructor

Loop::Loop(std::vector<Server> servers, std::vector<Socket *> sockets, int nbSockets)
: _sockets(sockets)
{
	_createMapServer(servers);
	this->_epoll = Epoll(sockets, nbSockets);
}

Loop::~Loop()
{

}

// METHODS
// PRIVATE

void	Loop::_createMapServer(std::vector<Server> servers)
{
	int	nbServers;

	nbServers = 0;
	for (int i = 0; i < nbServers; i++)
	{
		this->_servers[servers[i].getHostname()] = servers[i];
	}
}

void	Loop::_closeClients(int idClient)
{
	::epoll_ctl(this->_epoll.getEpollFd(), EPOLL_CTL_DEL, this->_clients[idClient]->getFdClient(), 0);
	this->_epoll.setNbSockets(this->_epoll.getNbSockets() - 1);

	if (this->_clients[idClient]->getFdClient() > -1)
		::close(this->_clients[idClient]->getFdClient());
	delete this->_clients[idClient];
	this->_clients.erase(this->_clients.begin() + idClient);
}

void	Loop::_checkAllTimeout()
{
	int	nbClients;

	nbClients = this->_clients.size();
	if (nbClients == 0)
		return ;
	for (int i = 0; i < nbClients; i++)
	{
		if (this->_clients[i]->checkTimeout())
		{
			this->_closeClients(i);
			nbClients = this->_clients.size();
			i--;
		}
	}
}

std::string	Loop::_createResponse(Client *client, Server serv)
{
	Methods *met;
	std::string method = client->getRequest().getMethod();

	if (method == "GET")
		met = new Get(client->getRequest());
	else if (method == "POST")
		met = new Post(client->getRequest());
	else
		met = new Delete(client->getRequest());

	return met->createResponse(serv);
}

inline void Loop::_sendResponse(Client *client, std::string response)
{
	if (send(client->getFdClient(), response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
}

bool	Loop::_isServerSocket(int fd)
{
	int	sizeSockets;
	int	nbSockData;

	sizeSockets = this->_sockets.size();
	for (int i = 0; i < sizeSockets; i++)
	{
		nbSockData = this->_sockets[i]->getSockData().size();
		for (int j = 0; j < nbSockData; j++)
		{
			if (fd == this->_sockets[i]->getSockData()[j]->getFdServer())
			{
				this->_hostnameOfSrvSock = this->_sockets[i]->getHostname();
				return (true);
			}
		}
	}
	return (false);
}

bool	Loop::_isClientSocket(int fd, int &idClient)
{
	int	sizeClients;

	sizeClients = this->_clients.size();
	for (int i = 0; i < sizeClients; i++)
	{
		if (fd == this->_clients[i]->getFdClient())
		{
			idClient = i;
			return (true);
		}
	}
	return (false);
}

int	Loop::_acceptClient(int fd)
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
	newClient->setHostname(this->_hostnameOfSrvSock);
	this->_clients.push_back(newClient);
	this->_epoll.addEpollFd(newClient->getFdClient(), EPOLLIN);
	return (0);
}

int	Loop::_receiveRequest(int idClient)
{
	int		sizeRecv;
	char	buffer[10000];

	sizeRecv = -1;
	sizeRecv = ::recv(this->_clients[idClient]->getFdClient(), buffer, sizeof(buffer) - 1, 0);
	if (sizeRecv == -1)
		return (this->_receiveRequest(idClient));
	buffer[sizeRecv] = '\0';
	std::cout << "sizeRecv : " << sizeRecv << std::endl;
	if (sizeRecv == 0)
		return (0);
	this->_clients[idClient]->setBuf(buffer, sizeRecv);
	if (this->_clients[idClient]->getBuf().find("\r\n\r\n") != std::string::npos)
	{
		// client->setEndOfFile(true);
		this->_epoll.setEvents(this->_clients[idClient], EPOLLOUT);
	}
	this->_clients[idClient]->setTimeoutRequest();
	this->_clients[idClient]->setTimeout();
	return (1);
}

// PUBLIC

void	Loop::runLoop()
{
	while (true)
	{
		int			idClient;
		int			epollCounterWait;
		epoll_event	events[this->_epoll.getNbSockets()];

		epollCounterWait = 0;
		idClient = 0;
	
		epollCounterWait = ::epoll_wait(this->_epoll.getEpollFd(), events, this->_epoll.getNbSockets(), 2000);
		std::cout << GREEN << "dana : " << epollCounterWait << std::endl << RESET;
		this->_checkAllTimeout();
		if (epollCounterWait < 1)
			continue ;
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && this->_isServerSocket(events[indexEvent].data.fd))
			{
				std::cout << "Just before accept : " << epollCounterWait << std::endl;
			
				if (this->_acceptClient(events[indexEvent].data.fd) < 0)
					continue ;
				for (size_t i = 0; i < this->_clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << this->_clients[i]->getFdClient() << ", " << this->_clients[i]->getHostname() << ", keep alive : " << this->_clients[i]->getKeepAlive() << std::endl << RESET;
				}
				std::cout << "epoll size : " << this->_epoll.getNbSockets() << std::endl;
			}
			std::cout << GREEN << "epollin : " << events[indexEvent].events << ", " << (this->_isClientSocket(events[indexEvent].data.fd, idClient)) << std::endl << RESET;
			if (events[indexEvent].events & EPOLLIN && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				std::cout << "receive" << std::endl;
				if (this->_receiveRequest(idClient) == 0)
				{
					std::cout << LIGHT_YELLOW << "Request : " << this->_clients[idClient]->getBuf() << std::endl << RESET;
					this->_closeClients(idClient);
					continue ;
				}
				std::cout << MAGENTA <<"Request : " << this->_clients[idClient]->getBuf() << std::endl << RESET;
			}
			// std::cout << "event : " << events[indexEvent].data.fd << ", " << events[indexEvent].events << ", " << isClientSocket(events[indexEvent].data.fd, clients, idClient) << std::endl;
			this->_clients[idClient]->checkTimeoutRequest();
			if (events[indexEvent].events & EPOLLOUT && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				std::cout << "chez kilian" << std::endl;
				std::cout << RED << "Request : " << this->_clients[idClient]->getBuf() << std::endl << RESET;
				std::cout << "maybe after" << std::endl;
				sendResponse(this->getClients()[idClient], this->getServer(this->_hostnameOfSrvSock));
				for (size_t i = 0; i < this->_clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << this->_clients[i]->getFdClient() << ", " << this->_clients[i]->getHostname() << ", keep alive : " << this->_clients[i]->getKeepAlive() << std::endl << RESET;
				}
				if (this->_clients[idClient]->getKeepAlive() == false)
					this->_closeClients(idClient);
				else
				{
					this->_clients[idClient]->resetClient();
					this->_epoll.setEvents(this->_clients[idClient], EPOLLIN);
				}
				std::cout << "end" << std::endl;
			}
		}
	}
}

std::vector<Client *> const	&Loop::getClients() const
{
	return (this->_clients);
}

Server const	&Loop::getServer(std::string hostname) const
{
	return (this->_servers.at(hostname));
}
