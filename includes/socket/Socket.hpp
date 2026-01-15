#ifndef SOCKET_HPP

#define SOCKET_HPP

#include <poll.h>
#include "SocketData.hpp"

class Socket
{
	private:
		std::string					_hostname;
		std::vector<SocketData *>	_sockData;

		void	addingSockets(server data);
	public:
		Socket(Server data, int i);
		~Socket();

		std::string	const				&getHostname() const;
		std::vector<SocketData *> const	&getSockData() const;

		void	setFdClient(int newFdClient,size_t indexSocketData);

		std::vector<SocketData *>::iterator	getBegin();
		std::vector<SocketData *>::iterator	getEnd();
		void								eraseSocket(int i);
};

#endif