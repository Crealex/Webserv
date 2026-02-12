#ifndef SOCKETDATA_HPP

#define SOCKETDATA_HPP

#include "../includes.hpp"
#include <netinet/in.h>

typedef std::pair<std::string, unsigned int> addPort_t;

class SocketData
{
	private:
		sockaddr_in	_sockadd;
		int			_fdServer;

		void	_sockOptNonBlocking(int &socketFd);

		uint32_t 	_ipToUint(std::string s);
		std::string	_intToIp();

		void	_assignmentSocket(addPort_t addPort);
	public:
		SocketData(addPort_t);
		~SocketData();

		sockaddr_in	const	&getSockadd() const;
		int	const			&getFdServer() const;

		void	printAddrPort();
};

#endif