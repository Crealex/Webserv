#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "includes.hpp"
#include "requests/Request.hpp"
#include "requests/CGI/CGI.hpp"
#include <cstddef>
#include <netinet/in.h>

#define MAXTIMEREQUEST 3
#define MAXTIME 30

class Client
{
	private:
		bool		_isCGI;
		std::string	_hostname;
		int			_fdSocket;
		std::string	_buf;
		sockaddr_in	_sockadd;
		Request		_request;
		CGI			_CGI;
		std::string	_response;
		std::time_t	_timeRequest;
		std::time_t	_time;


		std::string	_intToIp();
		int			_getSizeBody();
		void		_checkCGIResponse();

	public:
		Client();
		~Client();

		bool const			&getIsCGI() const;
		std::string const	&getHostname() const;
		int const			&getFDCGI() const;
		int const			&getFdClient() const;
		std::string const	&getBuf() const;
		sockaddr_in const	&getSockadd() const;
		Request	const		&getRequest() const;
		std::string const	&getResponse() const;

		void	setHostname(std::string newHostname);
		void	setFdClient(int newFd);
		void	setBuf(const char *newBuf, int size);
		void	setSockadd(sockaddr_in newSockadd);
		void	setRequestHeader(std::string &str);
		void	setRequestBody();
		void	setResponse(const std::string &str);
		void	setTimeoutRequest();
		void	setTimeout();
		
		std::time_t	getTimeNow();

		bool	checkCGI(Server &serv);
		void	isCGI(Server &serv);
		void	startCGI(Server &serv, Epoll &epoll);
		void	resetBuf();
		void	resetClient();
		void	checkRequest(Server server);
		bool	checkTimeoutRequest();
		bool	checkTimeout();

		void	printAddPort();
};

#endif
