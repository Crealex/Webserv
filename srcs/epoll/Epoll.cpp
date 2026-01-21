#include "../../includes/epoll/Epoll.hpp"

// CONSTRUCTOR & DESTRUCTOR
Epoll::Epoll(std::vector<Socket *> sockets, int nbSockets)
{
	this->_epollFd = createEpoll();
	this->_nbSockets = nbSockets;
	this->_events = addEpollServer(sockets, nbSockets, this->_epollFd);
}

Epoll::~Epoll()
{
	if (this->_events)
		delete[] this->_events;
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

epoll_event const	*Epoll::getEvents() const
{
	return (this->_events);
}

epoll_event	*Epoll::getEvents()
{
	return (this->_events);
}


// SETTERS
void	Epoll::setNbSockets(int newNb)
{
	this->_nbSockets = newNb;
}

void	Epoll::setEvents(epoll_event *newEvents)
{
	if (this->_events)
	{
		delete[] this->_events;
	}
	this->_events = newEvents;
}

// METHODS
// PRIVATE
int	Epoll::createEpoll()
{
	int	res;

	res = epoll_create(1);
	if (res == -1)
	{
		return (createEpoll());
	}
	return (res);
}

epoll_event	*Epoll::addEpollServer(std::vector<Socket *> &sockets, int sizeRes, int epollFd)
{
	epoll_event	*res;
	int			sizeSocket;
	int			sizeSockData;
	int			count;

	res = new epoll_event[sizeRes];
	sizeSocket = sockets.size();
	count = 0;
	for (int i = 0; i < sizeSocket; i++)
	{
		sizeSockData = sockets[i]->getSockData().size();
		for (int j = 0; j < sizeSockData; j++)
		{
			res[count].data.fd = sockets[i]->getSockData()[j]->getFdServer();
			res[count].events = EPOLLIN;
			if (epoll_ctl(epollFd, EPOLL_CTL_ADD, res[count].data.fd, &res[count]) < 0)
			{
				if (res)
					delete[] res;
				return (addEpollServer(sockets, sizeRes, epollFd));
			}
			count++;
		}
	}
	return (res);
}

// PUBLIC

void	Epoll::addEpollFd(int fd, uint32_t event)
{
	epoll_event	*temp;

	temp = new epoll_event[this->_nbSockets + 1];
	for (int i = 0; i < this->_nbSockets; i++)
		temp[i] = this->_events[i];

	temp[this->_nbSockets].data.fd = fd;
	temp[this->_nbSockets].events = event;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, temp[this->_nbSockets].data.fd, &temp[this->_nbSockets]) < 0)
	{
		if (temp)
		{
			delete[] temp;
		}
		addEpollFd(fd, event);
	}
	this->setEvents(temp);
	this->_nbSockets++;
}

void	Epoll::setEvents(Client *client, uint32_t event)
{
	int indexEvents;

	indexEvents = 0;
	for (int i = 0; i < this->_nbSockets; i++)
	{
		if (this->_events[i].data.fd == client->getFdClient())
		{
			indexEvents = i;
			break ;
		}
	}
	this->_events[indexEvents].events = event;
}