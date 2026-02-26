#include "../../includes/epoll/Epoll.hpp"

// CONSTRUCTOR & DESTRUCTOR
Epoll::Epoll()
{
	this->_epollFd = -1;
	this->_nbSockets = 0;
}

Epoll::Epoll(std::vector<Socket *> sockets)
{
	this->_epollFd = createEpoll();
	this->_nbSockets = 0;
	addEpollServer(sockets);
}

Epoll::~Epoll()
{
}

// GETTERS
int const			&Epoll::getEpollFd() const
{
	return (this->_epollFd);
}

int const			&Epoll::getNbSockets() const
{
	return (this->_nbSockets);
}

// SETTERS
void	Epoll::setNbSockets(int newNb)
{
	this->_nbSockets = newNb;
}

void	Epoll::setEvents(Client *client, uint32_t event)
{
	epoll_event	temp;

	temp.data.fd = client->getFdClient();
	temp.events = event;
	::epoll_ctl(this->getEpollFd(), EPOLL_CTL_MOD, temp.data.fd, &temp);
}


// METHODS
// PRIVATE
int	Epoll::createEpoll()
{
	int	res;

	res = ::epoll_create(1);
	if (res == -1)
	{
		return (createEpoll());
	}
	return (res);
}

void	Epoll::addEpollServer(std::vector<Socket *> &sockets)
{
	epoll_event	res;
	int			sizeSocket;
	int			sizeSockData;
	int			counter;
	int			epollStatus;

	sizeSocket = sockets.size();
	for (int i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < sizeSockData; j++)
		{
			counter = 0;
			do
			{
				res.data.fd = sockets[i]->getSockData()[j]->getFdServer();
				res.events = EPOLLIN;
				epollStatus = ::epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, res.data.fd, &res);
				counter++;
			} while (epollStatus < 0 && counter < 3);
			if (epollStatus > -1)
				this->_nbSockets++;
			// else : log error
		}
	}
}

// PUBLIC

void	Epoll::addEpollFd(int fd, uint32_t event)
{
	epoll_event	temp;
	int			counter;
	int			epollStatus;

	counter = 0;
	do
	{
		temp.data.fd = fd;
		temp.events = event;
		epollStatus = ::epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, temp.data.fd, &temp);
		counter++;
	} while (epollStatus < 0 && counter < 3);
	if (epollStatus > -1)
		this->_nbSockets++;
	// else : error log
}

void	Epoll::closeFd()
{
	if (this->_epollFd > -1)
		close (this->_epollFd);
}
