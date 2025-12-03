#ifndef SOCKETDATA_HPP

#define SOCKETDATA_HPP

#include "includes.hpp"
#include "Config.hpp"
#include <netinet/in.h>

class SocketData
{
	private:
		sockaddr_in	_sockadd;
		int			_fdServer;
		int			_fdClient;

		void	assignmentSocket(addPort_t addPort);
		void	sockOpt(int &socketFd);
	public:
		SocketData(addPort_t);
		~SocketData();

		sockaddr_in	getSockadd() const;
		int			getFdServer() const;
		int			getFdClient() const;
};

#endif