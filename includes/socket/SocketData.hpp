#ifndef SOCKETDATA_HPP

#define SOCKETDATA_HPP

#include "../includes.hpp"
#include "includeSockNonBlocking.hpp"
#include <netinet/in.h>

typedef std::pair<std::string, unsigned int> addPort_t;

class SocketData
{
	private:
		sockaddr_in	_sockadd;
		int			_fdServer;

		void	assignmentSocket(addPort_t addPort);
	public:
		SocketData(addPort_t);
		~SocketData();

		sockaddr_in	const	&getSockadd() const;
		int	const			&getFdServer() const;
};

#endif