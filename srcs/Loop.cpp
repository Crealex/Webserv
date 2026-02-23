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

void	Loop::_sockOptNonBlocking(int &socketFd)
{
		int	opt;

		opt = 1;
		::fcntl(socketFd, F_SETFL, O_NONBLOCK);
		::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}

void	Loop::_createMapServer(std::vector<Server> servers)
{
	int	nbServers;

	nbServers = servers.size();
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

bool	Loop::_isCGI(int fd, int &idClient)
{
	int	sizeClients;

	std::cout << RED << BOLD << "fd = " << fd << RESET << std::endl;
	sizeClients = this->_clients.size();
	for (int i = 0; i < sizeClients; i++)
	{
		if (fd == this->_clients[i]->getFDCGI())
		{
			idClient = i;
			return (true);
		}
	}
	return (false);
}

void	Loop::_acceptClient(int fd)
{
	int 		fdClient;
	Client		*newClient;
	sockaddr_in	newSockadd;
	socklen_t	addrlen;
	
	while (1)
	{
		fdClient = -1;
		addrlen = sizeof(newSockadd);
		fdClient = ::accept(fd, (sockaddr *)&newSockadd, &addrlen);
		if (fdClient == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			else
			{
				std::cerr << RED << "Error : fd client in accept" << std::endl << RESET;
				break ;
			}
		}
		this->_sockOptNonBlocking(fdClient);
		newClient = new Client();
		newClient->setFdClient(fdClient);
		newClient->setSockadd(newSockadd);
		newClient->setHostname(this->_hostnameOfSrvSock);
		this->_clients.push_back(newClient);
		this->_epoll.addEpollFd(newClient->getFdClient(), EPOLLIN|EPOLLET);
	}
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
	size_t	posCRLF;

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
	this->_clients[idClient]->setRequestBody();
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
	this->_epoll.setEvents(this->_clients[idClient], EPOLLOUT);
	this->_clients[idClient]->setTimeoutRequest();
	this->_clients[idClient]->setTimeout();
	return (true);
}

void	Loop::_createResponse(int idClient)
{
	Methods *met;
	std::string method = this->_clients[idClient]->getRequest().getMethod();

	try
	{
		this->_clients[idClient]->checkRequest(this->_servers.at(this->_clients[idClient]->getHostname()));
		if (method == "GET")
			met = new Get(this->_clients[idClient]->getRequest());
		else if (method == "POST")
			met = new Post(this->_clients[idClient]->getRequest());
		else
			met = new Delete(this->_clients[idClient]->getRequest());
	
		this->_clients[idClient]->setResponse(met->createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
		delete met;
	}
	catch(ResponseError& e)
	{
		this->_clients[idClient]->setResponse(e.createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
	}
}

void	Loop::_createTimeoutResponse(int idClient)
{
	try
	{
		if (this->_clients[idClient]->checkTimeoutRequest())
			throw ResponseError(504, "Error: Gateway Timeout", this->_clients[idClient]->getRequest());
	}
	catch (ResponseError& e)
	{
		this->_clients[idClient]->setResponse(e.createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
	}
}

inline void Loop::_sendResponse(int idClient)
{
	this->_createTimeoutResponse(idClient);
	while (send(this->_clients[idClient]->getFdClient(), this->_clients[idClient]->getResponse().c_str(),
		this->_clients[idClient]->getResponse().size(), MSG_NOSIGNAL) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
		this->_createTimeoutResponse(idClient);
	}
}

void	Loop::_printTime()
{
	std::time_t timeNow;
	struct tm	*timeDisplay;
	char		display[100];

	std::time(&timeNow);
	timeDisplay = std::localtime(&timeNow);
	std::strftime(display, sizeof(display), "Date: %a, %d.%m.%Y - %X", timeDisplay);
	std::cout << display;
}

void	Loop::_printSocket()
{
	size_t	nbSock = this->_sockets.size();
	size_t	sizeSocket;

	for (size_t i = 0; i < nbSock; i++)
	{
		sizeSocket = this->_sockets[i]->getSockData().size();
		for (size_t j = 0; j < sizeSocket; j++)
		{
			std::cout << GREEN;
			std::cout << "Listen on : ";
			this->_sockets[i]->getSockData()[j]->printAddrPort();
			std::cout << std::endl << RESET;
		}
	}
}

void	Loop::_printSend(int idClient)
{
	std::cout << CYAN << std::endl;
	std::cout << BOLD;
	std::cout << "SEND" << std::endl;
	std::cout << RESET << CYAN;
	this->_printTime();
	std::cout << std::endl;
	std::cout << "On : " << this->_clients[idClient]->getRequest().getHost();
	std::cout << std::endl << BOLD;
	std::cout << std::endl << "Request : " << std::endl;
	std::cout << RESET << CYAN;
	this->_clients[idClient]->getRequest().printRequest();
	std::cout << BOLD;
	std::cout << std::endl << "Response : " << std::endl;
	std::cout << RESET << CYAN;
	std::cout << this->_clients[idClient]->getResponse();
	std::cout << RESET;
	std::cout << std::endl;
}

// PUBLIC

void	Loop::runLoop()
{
	bool	printSocket;

	printSocket = true;
	while (true)
	{
		if (printSocket == true)
		{
			this->_printSocket();
			printSocket = false;
		}
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
				this->_acceptClient(events[indexEvent].data.fd);
			}
			else if (events[indexEvent].events & EPOLLIN && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				if (!this->_getRequest(idClient))
				{
					this->_closeClients(idClient);
					continue ;
				}
				this->_clients[idClient]->setTimeout();
				this->_clients[idClient]->setTimeoutRequest();
				_clients[idClient]->isCGI(_servers[_clients[idClient]->getHostname()]);
				if (_clients[idClient]->getIsCGI())
				{
					_clients[idClient]->startCGI(_servers[_clients[idClient]->getHostname()], _epoll);
				}
			}
			else if (_clients[idClient]->getIsCGI())
			{
				if (!_clients[idClient]->checkCGI(_servers[_clients[idClient]->getHostname()]))
					continue ;
				if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
				{
					// this->_printCloseClient(idClient);
					this->_closeClients(idClient);
				}
				else
				{
					this->_clients[idClient]->resetClient();
					this->_epoll.setEvents(this->_clients[idClient], EPOLLIN|EPOLLET);
				}
			}
			else if (events[indexEvent].events & EPOLLOUT && this->_isClientSocket(events[indexEvent].data.fd, idClient) && !_clients[idClient]->getIsCGI())
			{
				this->_createResponse(idClient);
				this->_sendResponse(idClient);
				this->_printSend(idClient);
				if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
				{
					this->_closeClients(idClient);
				}
				else
				{
					this->_clients[idClient]->resetClient();
					this->_epoll.setEvents(this->_clients[idClient], EPOLLIN|EPOLLET);
				}
				printSocket = true;
			}
		}
	}
}
