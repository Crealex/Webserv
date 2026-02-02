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

bool	Loop::_parsingRequest(int idClient)
{
	size_t		posCRLF;
	std::string	header;
	std::string	body;
	int			sizeBuf;

	posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	if (posCRLF == std::string::npos)
		return (false);
	header = this->_clients[idClient]->getBuf().substr(0, posCRLF + 4);
	this->_clients[idClient]->setRequestHeader(header);
	sizeBuf = this->_clients[idClient]->getBuf().size();
	body = this->_clients[idClient]->getBuf().substr(posCRLF + 4, sizeBuf - posCRLF - 4);
	this->_clients[idClient]->resetBuf();
	this->_clients[idClient]->setBuf(body.c_str(), sizeBuf - posCRLF - 4);
	return (true);
}

int	Loop::_receiveRequest(int idClient)
{
	int		sizeRecv;
	char	buffer[10000];

	sizeRecv = ::recv(this->_clients[idClient]->getFdClient(), buffer, sizeof(buffer) - 1, 0);
	if (sizeRecv == -1)
		return (this->_receiveRequest(idClient));
	buffer[sizeRecv] = '\0';
	if (sizeRecv == 0)	
		return (sizeRecv);
	this->_clients[idClient]->setBuf(buffer, sizeRecv);	
	return (sizeRecv);
}	

void	Loop::_addBodyLen(int idClient)
{
	int	counter;
	int	contentLen;

	counter = this->_clients[idClient]->getBuf().size();
	contentLen = this->_clients[idClient]->getRequest().getContentLength();
	while (counter < contentLen)
	{
		counter += this->_receiveRequest(idClient);
	}
}	

void	Loop::_addBodyChunked(int idClient)
{
	int	posCRLF;

	posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	while (posCRLF != std::string::npos)
	{
		this->_receiveRequest(idClient);
		posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	}
}	

void	Loop::_checkBody(int idClient)
{
	if (this->_clients[idClient]->getRequest().getContentLength() > 0)
		this->_addBodyLen(idClient);
	if (this->_clients[idClient]->getRequest().getTranferEncoding().find("chunked") != std::string::npos)	
		this->_addBodyChunked(idClient);
}		

bool	Loop::_getRequest(int idClient)
{
	int		recvStatus;

	recvStatus = this->_receiveRequest(idClient);
	if (recvStatus == 0)
		return (false);
	if (!this->_parsingRequest(idClient))
		this->_getRequest(idClient);	
	this->_checkBody(idClient);
	this->_clients[idClient]->setTimeoutRequest();
	this->_clients[idClient]->setTimeout();
	return (true);
}

inline void Loop::_sendResponse(Client *client, std::string response)
{
	if (send(client->getFdClient(), response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
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
		this->_checkAllTimeout();
		if (epollCounterWait < 1)
			continue ;
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && this->_isServerSocket(events[indexEvent].data.fd))
			{
				if (this->_acceptClient(events[indexEvent].data.fd) < 0)
					continue ;
			}
			if (events[indexEvent].events & EPOLLIN && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				if (!this->_getRequest(idClient))
				{
					this->_closeClients(idClient);
					continue ;
				}
			}
			if (events[indexEvent].events & EPOLLOUT && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				this->_sendResponse(this->getClients()[idClient]);
				if (this->_clients[idClient]->getKeepAlive() == false)
					this->_closeClients(idClient);
				else
				{
					this->_clients[idClient]->resetClient();
					this->_epoll.setEvents(this->_clients[idClient], EPOLLIN);
				}
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
