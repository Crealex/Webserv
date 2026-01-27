#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "includes.hpp"
#include <netinet/in.h>

#define MAXTIMEREQUEST 15
#define MAXTIME 30

class Client
{
	private:
		std::string	_hostname;
		int			_fdSocket;
		std::string	_buf;
		sockaddr_in	_sockadd;
		bool		_endOfFile;
		bool		_keepAlive;
		std::time_t	_timeRequest;
		std::time_t	_time;
		bool		_timeoutRequest;
	
	public:
		Client();
		~Client();

		std::string const	&getHostname() const;
		int const			&getFdClient() const;
		std::string const	&getBuf() const;
		sockaddr_in const	&getSockadd() const;
		bool const			&getEndOfFile() const;
		bool const			&getKeepAlive() const;
		bool const			&getTimeoutRequest() const;

		void	setHostname(std::string newHostname);
		void	setFdClient(int newFd);
		void	setBuf(char *newBuf, int size);
		void	setSockadd(sockaddr_in newSockadd);
		void	setEndOfFile(bool newEndOfFile);
		void	setKeepAlive(bool newKeepAlive);
		void	setTimeoutRequest();
		void	setTimeout();
		
		void	resetClient();
		void	checkTimeoutRequest();
		bool	checkTimeout();
};

#endif