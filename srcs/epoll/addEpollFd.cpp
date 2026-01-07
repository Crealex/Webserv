#include "../../includes/includes.hpp"
#include "../../includes/epoll/Epoll.hpp"
#include "../../includes/socket/includeSocket.hpp"

void	addEpollFd(int fd, int nbPollFd, Epoll epoll)
{
	epoll_event	*temp;

	temp = new epoll_event[nbPollFd + 1];
	for (int i = 0; i < nbPollFd; i++)
		temp[i] = epoll.getEvents()[i];

	temp[nbPollFd].data.fd = fd;
	temp[nbPollFd].events = EPOLLIN;
	if (epoll_ctl(epoll.getEpollFd(), EPOLL_CTL_ADD, temp[nbPollFd].data.fd, &temp[nbPollFd]) < 0)
	{
		if (temp)
		{
			delete[] temp;
		}
		addEpollFd(fd, nbPollFd, epoll);
	}
	epoll.setEvents(temp);
	epoll.setNbSockets(nbPollFd + 1);
}
