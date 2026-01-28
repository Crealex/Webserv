#include "../../includes/epoll/Epoll.hpp"

// CONSTRUCTOR & DESTRUCTOR
Epoll::Epoll()
{
	this->_epollFd = -1;
	this->_nbSockets = -1;
}

Epoll::Epoll(std::vector<Socket *> sockets, int nbSockets)
{
	this->_epollFd = createEpoll();
	this->_nbSockets = nbSockets;
	addEpollServer(sockets, nbSockets, this->_epollFd);
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

void	Epoll::addEpollServer(std::vector<Socket *> &sockets, int sizeRes, int epollFd)
{
	epoll_event	res;
	int			sizeSocket;
	int			sizeSockData;
	int			count;

	sizeSocket = sockets.size();
	count = 0;
	for (int i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < sizeSockData; j++)
		{
			res.data.fd = sockets[i]->getSockData()[j]->getFdServer();
			res.events = EPOLLIN;
			if (::epoll_ctl(epollFd, EPOLL_CTL_ADD, res.data.fd, &res) < 0)
			{
				this->addEpollServer(sockets, sizeRes, epollFd);
			}
			count++;
		}
	}
}

// PUBLIC

void	Epoll::addEpollFd(int fd, uint32_t event)
{
	epoll_event	temp;

	temp.data.fd = fd;
	temp.events = event;
	if (::epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, temp.data.fd, &temp) < 0)
	{
		this->addEpollFd(fd, event);
	}
	this->_nbSockets++;
}
