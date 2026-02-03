#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "includes.hpp"
#include "requests/Request.hpp"
#include <cstddef>
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
		Request		_request;
		std::string	_response;
		bool		_keepAlive;
		std::time_t	_timeRequest;
		std::time_t	_time;

	public:
		Client();
		~Client();

		std::string const	&getHostname() const;
		int const			&getFdClient() const;
		std::string const	&getBuf() const;
		sockaddr_in const	&getSockadd() const;
		Request	const		&getRequest() const;
		std::string const	&getResponse() const;
		bool const			&getKeepAlive() const;

		void	setHostname(std::string newHostname);
		void	setFdClient(int newFd);
		void	setBuf(const char *newBuf, int size);
		void	setSockadd(sockaddr_in newSockadd);
		void	setRequestHeader(std::string &str);
		void	setRequestBody();
		void	setResponse(const std::string &str);
		void	setKeepAlive(bool newKeepAlive);
		void	setTimeoutRequest();
		void	setTimeout();
		
		std::time_t	getTimeNow();

		void	resetBuf();
		void	resetClient();
		void	checkRequest(Server server);
		bool	checkTimeoutRequest();
		bool	checkTimeout();
};

#endif
