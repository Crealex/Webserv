#ifndef SOCKETDATA_HPP

#define SOCKETDATA_HPP

#include "../includes.hpp"
#include "../Server.hpp"
#include <netinet/in.h>

class SocketData
{
	private:
		sockaddr_in	_sockadd;
		int			_fdServer;
		int			_fdClient;

		void	assignmentSocket(addPort_t addPort);
		void	sockOptNonBlocking(int &socketFd);
	public:
		SocketData(addPort_t);
		~SocketData();

		sockaddr_in	const	&getSockadd() const;
		int	const			&getFdServer() const;
		int	const			&getFdClient() const;

		void	setFdClient(int newFdClient);
};

#endif