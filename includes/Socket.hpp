#ifndef SOCKET_HPP

#define SOCKET_HPP

#include "includes.hpp"
#include "Config.hpp"
#include <netinet/in.h>

class Socket
{
	private:
		std::string					_hostname;
		std::vector<sockaddr_in>	_sockaddrs;
		std::vector<int>			_fdServer;
		std::vector<int>			_fdClient;

		void	assignmentSocket(Config conf, int i);
		void	sockOpt(int &socketFd);

	public:
		Socket(Config conf, int i);
		~Socket();

		std::string					getHostname() const;
		std::vector<sockaddr_in>	getSockaddrs() const;
		std::vector<int>			getFdServer() const;
		std::vector<int>			getFdClient() const;

		void	setFdClient();
};

#endif