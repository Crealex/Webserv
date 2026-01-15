#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "includes.hpp"
#include <netinet/in.h>

class Client
{
	private:
		std::string	_hostname;
		int			_fdSocket;
		std::string	_buf;
		sockaddr_in	_sockadd;
		bool		_endOfFile;
	
	public:
		Client();
		~Client();

		std::string const	&getHostname() const;
		int const			&getFdClient() const;
		std::string const	&getBuf() const;
		sockaddr_in const	&getSockadd() const;
		bool const			&getEndOfFile() const;

		void	setHostname(std::string newHostname);
		void	setFdClient(int newFd);
		void	setBuf(char *newBuf);
		void	setSockadd(sockaddr_in newSockadd);
		void	setEndOfFile(bool newEndOfFile);
};

#endif