#include "../includes/Loop.hpp"
#include "../includes/Logger.hpp"
#include <cerrno>
#include <sstream>
#include <sys/epoll.h>

volatile bool g_exit = false;

void signalHandler(int signal)
{
	(void)signal;
	g_exit = true;
}

// Constructor & Destructor

Loop::Loop(std::vector<Server> servers)
{
	this->_createMapServer(servers);
	this->_createSocket(servers);
	if (this->_sockets.size() == 0)
		throw std::runtime_error(RED "Error : no sockets to start the webserver" RESET);
	this->_epoll = Epoll(this->_sockets);
	this->_isExit = false;
}

Loop::~Loop()
{
	int sizeSock;
	int sizeSockData;
	int nbClients;

	sizeSock = this->_sockets.size();
	nbClients = this->_clients.size();
	for (int i = 0; i < sizeSock; i++)
	{
		sizeSockData = this->_sockets[i]->getSockData().size();
		if (this->_sockets[i])
		{
			for (int j = 0; j < sizeSockData; j++)
			{
				::epoll_ctl(this->_epoll.getEpollFd(), EPOLL_CTL_DEL, this->_sockets[i]->getSockData()[j]->getFdServer(), 0);
			}
			delete this->_sockets[i];
			this->_sockets[i] = NULL;
		}
	}
	for (int i = 0; i < nbClients; i++)
	{
		if (this->_clients[i])
		{
			::epoll_ctl(this->_epoll.getEpollFd(), EPOLL_CTL_DEL, this->_clients[i]->getFdClient(), 0);
			delete this->_clients[i];
			this->_clients[i] = NULL;
		}
	}
	this->_epoll.closeFd();
}

// METHODS
// PRIVATE

void Loop::_sockOptNonBlocking(int &socketFd)
{
	int opt;

	opt = 1;
	::fcntl(socketFd, F_SETFL, O_NONBLOCK);
	::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}

void Loop::_createMapServer(std::vector<Server> servers)
{
	int nbServers;

	nbServers = servers.size();
	for (int i = 0; i < nbServers; i++)
	{
		this->_servers[servers[i].getHostname()] = servers[i];
	}
}

void Loop::_listenSocket(size_t &i, size_t &j, size_t &sizeSockData)
{
	int checkFail;
	std::vector<SocketData *>::iterator eltToErase;

	checkFail = 0;
	checkFail = ::listen(this->_sockets[i]->getSockData()[j]->getFdServer(), 2);
	if (checkFail < 0)
	{
		this->_sockets[i]->eraseSocket(j);
		j--;
		sizeSockData--;
	}
}

void Loop::_bindSocket()
{
	int checkFail;
	size_t sizeSockets;
	size_t sizeSockData;

	checkFail = 0;
	sizeSockets = this->_sockets.size();
	for (size_t i = 0; i < sizeSockets; i++)
	{
		sizeSockData = this->_sockets[i]->getSockData().size();
		for (size_t j = 0; j != sizeSockData; j++)
		{
			checkFail = ::bind(this->_sockets[i]->getSockData()[j]->getFdServer(), (struct sockaddr *)&(this->_sockets[i]->getSockData()[j]->getSockadd()), sizeof(sockaddr_in));
			if (checkFail < 0)
			{
				this->_sockets[i]->eraseSocket(j);
				j--;
				sizeSockData--;
			} else
			{
				this->_listenSocket(i, j, sizeSockData);
			}
		}
		if (this->_sockets[i]->getSockData().size() == 0)
		{
			delete this->_sockets[i];
			this->_sockets[i] = NULL;
			this->_sockets.erase(this->_sockets.begin() + i);
			i--;
			sizeSockets--;
		}
	}
}

void Loop::_createSocket(std::vector<Server> srvs)
{
	size_t sizeSrvs;

	sizeSrvs = srvs.size();
	for (size_t i = 0; i < sizeSrvs; i++)
	{
		Socket *temp = new Socket(srvs[i]);
		this->_sockets.push_back(temp);
	}
	this->_bindSocket();
}

void Loop::_displayHelp()
{
	std::cout << "----------------------------------------------------\n\
	\rWelcome to our beautiful webserver! You can enter these cmd:\n\n\
	h - to display this message\n\
	q - to quit the webserv\n\
	l - to enable or disable the logs\n\
	ap - to display the address/port open\n\
	c - to dispaly a beautifl cat in ascii art\n\
	\r----------------------------------------------------"
			  << std::endl;
}

void Loop::_handleCmd()
{
	std::string in;
	char buffer[256];
	size_t bytesRead;
	static std::string inputBuffer;

	bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytesRead <= 0)
		return;

	buffer[bytesRead] = '\0';
	inputBuffer += buffer;
	size_t pos = inputBuffer.find('\n');
	if (pos == std::string::npos)
		return;
	in = inputBuffer.substr(0, pos);
	inputBuffer.erase(0, pos + 1);

	if (in == "h" || in == "help")
		this->_displayHelp();
	else if (in == "q")
		this->_isExit = true;
	else if (in == "l")
	{
		if (Logger::getIsEnabled())
		{
			Logger::setIsEnabled(0);
			std::cout << "logs disabled" << std::endl;
		} else
		{
			Logger::setIsEnabled(1);
			std::cout << "logs enabled" << std::endl;
		}
	} else if (in == "ap")
		this->_printSocket();
	else if (in == "c")
		std::cout << LIGHT_MAGENTA << CAT << RESET << std::endl;
}

void Loop::_closeClients(int idClient)
{
	::epoll_ctl(this->_epoll.getEpollFd(), EPOLL_CTL_DEL, this->_clients[idClient]->getFdClient(), 0);
	this->_epoll.setNbSockets(this->_epoll.getNbSockets() - 1);

	delete this->_clients[idClient];
	this->_clients.erase(this->_clients.begin() + idClient);
}

void Loop::_checkAllTimeout()
{
	int nbClients;

	nbClients = this->_clients.size();
	if (nbClients == 0)
		return;
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

bool Loop::_isServerSocket(int fd)
{
	int sizeSockets;
	int nbSockData;

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

bool Loop::_isClientSocket(int fd, int &idClient)
{
	int sizeClients;

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

bool Loop::_isCGI(int fd, int &idClient)
{
	int sizeClients;

	//std::cout << RED << BOLD << "fd = " << fd << RESET << std::endl;
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

void Loop::_acceptClient(int fd)
{
	int fdClient;
	Client *newClient;
	sockaddr_in newSockadd;
	socklen_t addrlen;

	while (1)
	{
		fdClient = -1;
		addrlen = sizeof(newSockadd);
		fdClient = ::accept(fd, (sockaddr *)&newSockadd, &addrlen);
		if (fdClient == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
			{
				std::cerr << RED << "Error : fd client in accept" << std::endl
						  << RESET;
				break;
			}
		}
		this->_sockOptNonBlocking(fdClient);
		newClient = new Client();
		newClient->setFdClient(fdClient);
		newClient->setSockadd(newSockadd);
		newClient->setHostname(this->_hostnameOfSrvSock);
		this->_clients.push_back(newClient);
		this->_epoll.addEpollFd(newClient->getFdClient(), EPOLLIN);
	}
}

int Loop::_receive(int idClient)
{
	int sizeRecv;
	char buffer[10000];
	std::string bufferStr;
	
	std::memset(buffer, '\0',10000);
	printf("before buffer : %s\n", buffer);
	sizeRecv = ::recv(this->_clients[idClient]->getFdClient(), buffer, 9999, 0);
	// if (sizeRecv == -1)
	// {
	// 	buffer[0] = '\0';
	// 	return (sizeRecv);
	// }
	buffer[sizeRecv] = '\0';
	if (sizeRecv == 0)
		return (sizeRecv);
	this->_clients[idClient]->setBuf(buffer, sizeRecv);
	bufferStr = buffer;
	Logger::log(Logger::INFO, "REQUEST:\n" + bufferStr);
	return (sizeRecv);
}

// void Loop::_addBodyLen(int idClient)
// {
// 	int counter;
// 	int sizeRecv;
// 	int contentLen;
	
// 	counter = this->_clients[idClient]->getBuf().size();
// 	printf("counter : %i\n", counter);
// 	contentLen = this->_clients[idClient]->getRequest().getContentLength();
// 	while (counter < contentLen)
// 	{
// 		sizeRecv = this->_receive(idClient);
// 		printf("counter : %i, size recv : %i\n", counter, sizeRecv);
// 		if (sizeRecv <= 0)
// 		{
// 			::sleep(1);
// 		}
// 		else
// 			counter += sizeRecv;
// 	}
// 	printf("counter : %i\n", counter);
// }

// void Loop::_addBodyChunked(int idClient, size_t &posCRLF)
// {
// 	int sizeRecv;
	
// 	posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
// 	while (posCRLF != std::string::npos)
// 	{
// 		sizeRecv = this->_receive(idClient);
// 		if (sizeRecv <= 0)
// 		break ;
// 		posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
// 	}
// }

void Loop::_checkBody(int idClient)
{
	size_t posCRLF;

	if (this->_clients[idClient]->getRequest().getContentLength() > 0)
	{
		std::cout << "in content length : " << this->_clients[idClient]->getRequest().getContentLength() << ", fd client : " << idClient << std::endl;
		// this->_addBodyLen(idClient);
		if (this->_clients[idClient]->getBuf().size() == this->_clients[idClient]->getRequest().getContentLength())
			this->_clients[idClient]->settingRequestStatus(READY);
	}
	else if (this->_clients[idClient]->getRequest().getTranferEncoding().find("chunked") != std::string::npos)
	{
		std::cout << "chunked" << std::endl;
		// this->_addBodyChunked(idClient, posCRLF);
		posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
		if (posCRLF != std::string::npos)
			this->_clients[idClient]->settingRequestStatus(READY);
	}
	else
		this->_clients[idClient]->settingRequestStatus(READY);
}

void Loop::_parsingRequest(int idClient) // A REVOIR AVEC KILIAN
{
	size_t		posCRLF;
	std::string	header;
	std::string	body;
	int 		sizeBuf;

	posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	if (this->_clients[idClient]->getRequest().getStatus() == NOTHING)
	{
		if (posCRLF != std::string::npos)
		{
			this->_clients[idClient]->settingRequestStatus(CRLFFOUND);
		}
	}
	if (this->_clients[idClient]->getRequest().getStatus() == CRLFFOUND)
	{
		header = this->_clients[idClient]->getBuf().substr(0, posCRLF + 4);
		sizeBuf = this->_clients[idClient]->getBuf().size();
		body = this->_clients[idClient]->getBuf().substr(posCRLF + 4, sizeBuf - posCRLF - 4);
		this->_clients[idClient]->resetBuf();
		this->_clients[idClient]->setBuf(body.c_str(), sizeBuf - posCRLF - 4);
		this->_clients[idClient]->setRequestHeader(header);
		this->_clients[idClient]->settingRequestStatus(PARSINGHEADERDONE);
	}
	if (this->_clients[idClient]->getRequest().getStatus() == PARSINGHEADERDONE)
	{
		this->_checkBody(idClient);
	}
	if (this->_clients[idClient]->getRequest().getStatus() == READY)
		this->_clients[idClient]->setRequestBody();
}

bool Loop::_receiveRequest(int idClient)
{
	int		recvStatus;

	recvStatus = this->_receive(idClient);
	if (recvStatus == 0)
		return (false);
	this->_parsingRequest(idClient);
	if (this->_clients[idClient]->getRequest().getStatus() == READY)
		this->_epoll.setEvents(this->_clients[idClient], EPOLLOUT);
	return (true);
}

void Loop::_createResponse(int idClient)
{
	Methods *met;
	met = NULL;
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
	} catch (ResponseError &e)
	{
		this->_clients[idClient]->setResponse(e.createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
		if (met)
			delete met;
	}
	this->_clients[idClient]->setTimeout();
}

void Loop::_createTimeoutResponse(int idClient)
{
	try
	{
		if (this->_clients[idClient]->checkTimeoutRequest())
			throw ResponseError(408, "Request Timeout", this->_clients[idClient]->getRequest());
	} catch (ResponseError &e)
	{
		this->_clients[idClient]->setResponse(e.createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
	}
	this->_clients[idClient]->setTimeout();
}

inline void Loop::_sendResponse(int idClient)
{
	int	sizeSend;
	int	sizeResp;
	std::string	newResp;

	sizeSend = 0;
	printf("SIIIIIIIIIZE : %lu\n", this->_clients[idClient]->getResponse().size());
	sizeResp = this->_clients[idClient]->getResponse().size();
	std::cout << "fd client in send = " << _clients[idClient]->getFdClient() << std::endl;
	this->_createTimeoutResponse(idClient);
	sizeSend = send(this->_clients[idClient]->getFdClient(),
		this->_clients[idClient]->getResponse().data(),
		this->_clients[idClient]->getResponse().size(), MSG_NOSIGNAL);

		printf("size send : %i, resp size : %i\n", sizeSend, sizeResp);
	// if (sizeSend == -1)
	// 	return ;

	if (sizeSend >= sizeResp)
	{
		this->_clients[idClient]->setIsSend(true);
		return ;
	}
	newResp = this->_clients[idClient]->getResponse().substr(sizeSend, sizeResp - sizeSend);
	printf("size new resp : %lu\n", newResp.size());
	this->_clients[idClient]->setResponse(newResp);
	printf("size new resp : %lu\n", this->_clients[idClient]->getResponse().size());
}

void Loop::_printSocket()
{
	if (!SOCKET)
		return;
	size_t nbSock = this->_sockets.size();

	for (size_t i = 0; i < nbSock; i++)
	{
		std::cout << GREEN;
		std::cout << "Listen on : " << std::endl;
		this->_sockets[i]->printSockData();
		std::cout << RESET << std::endl;
	}
}

void Loop::_printSend(int idClient)
{
	std::stringstream respSS;

	if (!this->_clients[idClient]->getIsSend())
		return;
	respSS << BOLD << "RESPONSE:\n"
		   << RESET << this->_clients[idClient]->getResponse();
	Logger::log(Logger::INFO, respSS.str());
}

// PUBLIC

void Loop::runLoop()
{
	int counter;

	counter = 0;
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);				  // INFO: Ajouter par alex pour gerer les cmds
	this->_epoll.addEpollFd(STDIN_FILENO, EPOLLIN); // Comme ci dessus ^^^
	this->_displayHelp();
	while (!this->_isExit && !g_exit)
	{
		int idClient;
		int epollCounterWait;
		epoll_event events[this->_epoll.getNbSockets()];

		epollCounterWait = 0;
		idClient = 0;

		epollCounterWait = ::epoll_wait(this->_epoll.getEpollFd(), events, this->_epoll.getNbSockets(), 2000);
		this->_checkAllTimeout();
		if (epollCounterWait < 1)
		{
			if (epollCounterWait < 0)
			{
				if (counter > 3)
					this->_isExit = true;
				counter++;
			}
			continue;
		}
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && events[indexEvent].data.fd == STDIN_FILENO)
			{
				this->_handleCmd();
				if (this->_isExit)
					break;
				continue;
			} else if (events[indexEvent].events & EPOLLIN && this->_isServerSocket(events[indexEvent].data.fd))
			{
				std::cout << "before accept" << std::endl;
				this->_acceptClient(events[indexEvent].data.fd);
			} else if (events[indexEvent].events & EPOLLIN && this->_isClientSocket(events[indexEvent].data.fd, idClient))
			{
				std::cout << "fd client : " << this->_clients[idClient]->getFdClient() << std::endl;
				if (this->_receiveRequest(idClient) == false)
				{
					std::cout << "close client because getRequest return false" << std::endl;
					this->_closeClients(idClient);
					continue;
				}
				this->_clients[idClient]->setTimeout();
				this->_clients[idClient]->setTimeoutRequest();
				_clients[idClient]->isCGI(_servers[_clients[idClient]->getHostname()]);
				if (_clients[idClient]->getIsCGI())
				{
					_clients[idClient]->startCGI(_servers[_clients[idClient]->getHostname()]);
				}
				this->_createResponse(idClient);
			} else if (this->_isClientSocket(events[indexEvent].data.fd, idClient) && this->_clients[idClient]->getIsCGI())
			{
				if (!_clients[idClient]->checkCGI(_servers[_clients[idClient]->getHostname()]))
					continue;
				this->_sendResponse(idClient);
				this->_printSend(idClient);
				if (this->_clients[idClient]->getIsSend())
				{
					if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
					{
						this->_closeClients(idClient);
					} else
					{
						this->_clients[idClient]->resetClient();
						this->_epoll.setEvents(this->_clients[idClient], EPOLLIN);
					}
				}
			} else if (events[indexEvent].events & EPOLLOUT && this->_isClientSocket(events[indexEvent].data.fd, idClient) && !_clients[idClient]->getIsCGI())
			{
				this->_sendResponse(idClient);
				this->_printSend(idClient);
				printf("after\n");
				if (this->_clients[idClient]->getIsSend())
				{
					if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
					{
						this->_closeClients(idClient);
					} else
					{
						this->_clients[idClient]->resetClient();
						this->_epoll.setEvents(this->_clients[idClient], EPOLLIN);
					}
				}
			}
		}
	}
}
