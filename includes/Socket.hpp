#ifndef SOCKET_HPP

#define SOCKET_HPP

#include "SocketData.hpp"

class Socket
{
	private:
		std::string					_hostname;
		std::vector<SocketData>		_sockData;

		void	addingSockets(serverData data);
	public:
		Socket(serverData data);
		~Socket();

		std::string	const				&getHostname() const;
		std::vector<SocketData>			getSockData();
		std::vector<SocketData> const	&getSockData() const;

		void	setFdClient();
};

#endif