#ifndef	EPOLL_HPP

#define EPOLL_HPP

#include "../socket/Socket.hpp"
#include "../Client.hpp"
#include <sys/epoll.h>
#include "../Client.hpp"

class Epoll
{
	private:
		int			_epollFd;
		int			_nbSockets;

		int		createEpoll();
		void	addEpollServer(std::vector<Socket *> &sockets, int sizeRes, int epollFd);

	public:
		Epoll();
		Epoll(std::vector<Socket *> sockets, int nbSockets);
		~Epoll();

		int const			&getEpollFd() const;
		int const			&getNbSockets() const;
		
		void	setNbSockets(int newNb);
		void	setEvents(Client *client, uint32_t event);

		void	addEpollFd(int fd, uint32_t event);
};

#endif
