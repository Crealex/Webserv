#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "includes.hpp"
#include <netinet/in.h>

class Client
{
	private:
		int			_fdSocket;
		std::string	_buf;
		sockaddr_in	_sockadd;
		bool		_endOfFile;
	
	public:
		Client();
		~Client();

		int const			getFdClient() const;
		std::string const	getBuf() const;
		sockaddr_in const	getSockadd() const;

		void	setFdClient(int newFd);
		void	setBuf(char *newBuf);
		void	setSockadd(sockaddr_in newSockadd);
};

#endif