#include "../includes/Loop.hpp"
#include <sstream>
#include <sys/epoll.h>
#include "../includes/Logger.hpp"

volatile bool g_exit = false;

void	signalHandler(int signal)
{
	(void)signal;
	g_exit = true;
}

// Constructor & Destructor

Loop::Loop(std::vector<Server> servers)
{
	int	nbSockets;
	this->_createMapServer(servers);
	this->_createSocket(servers, nbSockets);
	if (this->_sockets.size() == 0)
		throw std::runtime_error(RED "Error : no sockets to start the webserver" RESET);
	this->_epoll = Epoll(this->_sockets, nbSockets);
	this->_isExit = false;
}

Loop::~Loop()
{
	int	sizeSock;
	int	sizeSockData;
	int	nbClients;
	
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

int	Loop::_listenSocket(size_t &i, size_t &j, size_t &sizeSockData)
{
	int									checkFail;
	std::vector<SocketData *>::iterator	eltToErase;

	checkFail = 0;
	checkFail = ::listen(this->_sockets[i]->getSockData()[j]->getFdServer(), 2);
	if (checkFail < 0)
	{
		this->_sockets[i]->eraseSocket(j);
		j--;
		sizeSockData--;
		return (0);
	}
	return (1);
}

int	Loop::_bindSocket()
{
	int		checkFail;
	int		nbSockets;
	size_t	sizeSockets;
	size_t	sizeSockData;

	checkFail = 0;
	nbSockets = 0;
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
			}
			else
			{
				nbSockets += this->_listenSocket(i, j, sizeSockData);
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
	return (nbSockets);
}

void	Loop::_createSocket(std::vector<Server> srvs, int &nbSockets)
{
	size_t	sizeSrvs;

	sizeSrvs = srvs.size();
	for (size_t i = 0; i < sizeSrvs; i++)
	{
		Socket	*temp = new Socket(srvs[i]);
		this->_sockets.push_back(temp);
	}
	nbSockets = this->_bindSocket();
}

void	Loop::_displayHelp()
{
	std::cout << "----------------------------------------------------\n\
	\rWelcome to our beautiful webserver! You can enter these cmd:\n\n\
	h - to display this message\n\
	q - to quit the webserv\n\
	l - to enable or disable the logs\n\
	ap - to display the address/port open\n\
	c - to dispaly a beautifl cat in ascii art\n\
	\r----------------------------------------------------" << std::endl;
}

void	Loop::_handleCmd()
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
		}
		else
		{
			Logger::setIsEnabled(1);
			std::cout << "logs enabled" << std::endl;
		}
	}
	else if (in == "ap")	
		this->_printSocket();
	else if (in == "c")	
		std::cout << LIGHT_MAGENTA << "\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣄⡀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡸⠋⠀⠘⣇⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠇⠀⠀⠀⢸⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡜⠀⠀⠀⠀⢸⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⠇⠀⠀⠀⠀⢸⠇⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡎⠀⠀⠀⠀⠀⢸⠀⠀⠀\n\
⠀⠀⢀⣀⣀⣀⠀⠀⠀⠀⠀⢀⣀⣤⡤⠤⠤⠤⠤⢤⣤⣀⡤⢖⡿⠛⠉⢳⠀⠀⠀⠀⠀⢸⠀⠀⠀\n\
⠀⢼⠁⠉⠉⠛⠻⢭⡓⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣏⠀⠀⠀⢸⠀⠀⠀⠀⠀⡤⠀⠀⠀\n\
⠀⠸⡄⠀⠀⠀⠀⢸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠂⠀⠀⡜⠀⠀⠀⠀⢀⡇⠀⠀⠀\n\
⠀⠀⢷⠀⠀⠀⠠⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⢠⠏⠀⠀⠀⠀⢸⠃⠀⠀⠀\n\
⠀⠀⠈⢧⠀⢀⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡞⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀\n\
⠀⠀⠀⠈⢳⡈⠁⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⣶⣶⣦⠀⠀⢹⠀⠀⠀⠀⠀⡎⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⢠⣾⣟⣹⡄⠀⠀⠀⠀⡀⠀⣿⣿⣿⡇⠀⢈⣧⠤⠤⠶⠶⢷⠒⠒⠂⠀\n\
⠀⠀⢀⣀⣠⡧⠄⠀⠀⠀⣾⣿⣿⣿⠇⠀⠀⠀⠙⠁⠀⠙⠻⠿⠃⠀⠨⣼⣤⣀⡀⠀⠈⢧⠀⠀⠀\n\
⠘⠉⠁⠀⢸⣤⡤⠀⠀⠀⠛⢿⡿⠋⠀⠀⠀⠀⠴⠦⠀⠀⠀⠀⠀⠐⣲⣯⡀⠀⠈⠙⠓⠺⣧⣄⡀\n\
⠀⣀⡤⠚⠉⢳⡴⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠃⠀⠈⠓⢦⡀⠀⠀⢸⠀⠈\n\
⠀⠁⠀⢀⡔⠉⠙⡶⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠴⠚⠁⠀⠀⠀⠀⠀⠀⠈⠓⠆⠀⡇⠀\n\
⠀⠀⠰⠋⠀⠀⢸⡇⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠁⠀\n\
⠀⠀⠀⠀⠀⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡎⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠙⢆⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠄⠀⢰⠇⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠶⠺⣇⠀⣀⡜⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢱⡄⠀⠀⠀⠹⡟⠒⢢⡀⠀⠀⠀⠀⢀⡏⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣄⠀⠀⢀⡇⠀⠀⠻⣄⠀⠀⠀⡸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n\
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢷⠶⠋⠀⠀⠀⠀⠈⣣⠶⠖⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" << RESET << std::endl;
}

void	Loop::_closeClients(int idClient)
{
	::epoll_ctl(this->_epoll.getEpollFd(), EPOLL_CTL_DEL, this->_clients[idClient]->getFdClient(), 0);
	this->_epoll.setNbSockets(this->_epoll.getNbSockets() - 1);

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
	{
		buffer[0] = '\0';
		return (sizeRecv);
	}
	buffer[sizeRecv] = '\0';
	if (sizeRecv == 0)	
		return (sizeRecv);
	this->_clients[idClient]->setBuf(buffer, sizeRecv);	
	return (sizeRecv);
}	

int	Loop::_addBodyLen(int idClient)
{
	int	counter;
	int	sizeRecv;
	int	contentLen;
	
	counter = this->_clients[idClient]->getBuf().size();
	contentLen = this->_clients[idClient]->getRequest().getContentLength();
	while (counter < contentLen)
	{
		sizeRecv = this->_receiveRequest(idClient);
		if (sizeRecv == -1)
			return (sizeRecv);
		counter += sizeRecv;
	}
	return (0);
}	

int	Loop::_addBodyChunked(int idClient)
{
	size_t	posCRLF;
	int		sizeRecv;

	posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	while (posCRLF != std::string::npos)
	{
		sizeRecv = this->_receiveRequest(idClient);
		if (sizeRecv == -1)
			return (sizeRecv);
		posCRLF = this->_clients[idClient]->getBuf().find("\r\n\r\n");
	}
	return (0);
}	

int	Loop::_checkBody(int idClient)
{
	if (this->_clients[idClient]->getRequest().getContentLength() > 0)
		if (this->_addBodyLen(idClient) == -1)
			return (-1);
	if (this->_clients[idClient]->getRequest().getTranferEncoding().find("chunked") != std::string::npos)	
		if (this->_addBodyChunked(idClient) == -1)
			return (-1);
	this->_clients[idClient]->setRequestBody();
	return (0);
}		

bool	Loop::_getRequest(int idClient)
{
	int		recvStatus;
	
	recvStatus = this->_receiveRequest(idClient);
	if (recvStatus <= 0)
		return (false);
	if (!this->_parsingRequest(idClient))
		this->_getRequest(idClient);	
	if (this->_checkBody(idClient) == -1)
		return (false);
	this->_epoll.setEvents(this->_clients[idClient], EPOLLOUT);
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
		if (met)
			delete met;
	}
	this->_clients[idClient]->setTimeout();
}

void	Loop::_createTimeoutResponse(int idClient)
{
	try
	{
		if (this->_clients[idClient]->checkTimeoutRequest())
			throw ResponseError(408, "Error: Request Timeout", this->_clients[idClient]->getRequest());
	}
	catch (ResponseError& e)
	{
		this->_clients[idClient]->setResponse(e.createResponse(this->_servers.at(this->_clients[idClient]->getHostname())));
	}
	this->_clients[idClient]->setTimeout();
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

void	Loop::_printSocket()
{
	if (!SOCKET)
		return ;
	size_t	nbSock = this->_sockets.size();

	for (size_t i = 0; i < nbSock; i++)
	{
		std::cout << GREEN;
		std::cout << "Listen on : " << std::endl;
		this->_sockets[i]->printSockData();
		std::cout << RESET << std::endl;
	}
}

void	Loop::_printSend(int idClient)
{
	std::stringstream respSS;
	std::stringstream requSS;
	if (!SEND)
		return ;
	respSS << BOLD << "RESPONSE:\n" << RESET << this->_clients[idClient]->getResponse();
	Logger::log(Logger::INFO, respSS.str());
	requSS << BOLD << "REQUEST:\n" << RESET << this->_clients[idClient]->getRequest().getRawRequest();
	Logger::log(Logger::INFO, requSS.str());
	//std::cout << CYAN << std::endl;
	//std::cout << BOLD;
	//std::cout << "SEND" << std::endl;
	//std::cout << "On : " << this->_clients[idClient]->getRequest().getHost();
	//std::cout << std::endl << BOLD;
	//Logger::_printTime();
	//std::cout << "\nRequest : " << std::endl;
	//std::cout << RESET << CYAN;
	//this->_clients[idClient]->getRequest().printRequest();
	//std::cout << BOLD;
	//std::cout << std::endl << "Response : " << std::endl;
	//std::cout << RESET << CYAN;
	//std::cout << this->_clients[idClient]->getResponse();
	//std::cout << RESET;
	//std::cout << std::endl;
}

// PUBLIC

void	Loop::runLoop()
{
	int	counter;
	// int nbrCGI = 0;
	counter = 0;
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // INFO: Ajouter par alex pour gerer les cmds
	this->_epoll.addEpollFd(STDIN_FILENO, EPOLLIN|EPOLLET);// Comme ci dessus ^^^
	this->_displayHelp();
	while (!this->_isExit && !g_exit)
	{
		int			idClient;
		int			epollCounterWait;
		epoll_event	events[this->_epoll.getNbSockets()];


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
			continue ;
		}
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && events[indexEvent].data.fd == STDIN_FILENO) // INFO: Condition ajouter par alex pour gérer les cmds
			{
				this->_handleCmd();
				if (this->_isExit)
					break ;
				continue;
			}
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
					_clients[idClient]->startCGI(_servers[_clients[idClient]->getHostname()]);
				}
			}
			else if (_clients[idClient]->getIsCGI())
			{
				if (!_clients[idClient]->checkCGI(_servers[_clients[idClient]->getHostname()]))
					continue ;
				if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
				{
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
				if (this->_clients[idClient]->getRequest().getkeepAlive() == false)
				{
					this->_closeClients(idClient);
				}
				else
				{
					this->_clients[idClient]->resetClient();
					this->_epoll.setEvents(this->_clients[idClient], EPOLLIN|EPOLLET);
				}
			}
		}
	}
}
