#ifndef	EPOLL_HPP

#define EPOLL_HPP

#include "../socket/Socket.hpp"
#include <sys/epoll.h>

class Epoll
{
	private:
		int			_epollFd;
		int			_nbSockets;
		epoll_event	*_events;

		int	createEpoll();
		epoll_event	*addEpollServer(std::vector<Socket *> &sockets, int sizeRes, int epollFd);

	public:
		Epoll(std::vector<Socket *> sockets, int nbSockets);
		~Epoll();

		int const			&getEpollFd() const;
		int const			&getNbSockets() const;
		epoll_event const	*getEvents() const;
		epoll_event 		*getEvents();
		
		void	setNbSockets(int newNb);
		void	setEvents(epoll_event *newEvents);
};

#endif
