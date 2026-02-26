#ifndef	EPOLL_HPP

#define EPOLL_HPP

#include "../socket/Socket.hpp"
#include "../Client.hpp"

class Client;

class Epoll
{
	private:
		int			_epollFd;
		int			_nbSockets;

		int		createEpoll();
		void	addEpollServer(std::vector<Socket *> &sockets);

	public:
		Epoll();
		Epoll(std::vector<Socket *> sockets);
		~Epoll();

		int const	&getEpollFd() const;
		int const	&getNbSockets() const;
		
		void	setNbSockets(int newNb);
		void	setEvents(Client *client, uint32_t event);

		void	addEpollFd(int fd, uint32_t event);
		void	closeFd();
};

#endif
