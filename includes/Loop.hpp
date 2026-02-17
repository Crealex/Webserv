#ifndef LOOP_HPP

#define LOOP_HPP

#include "includes.hpp"
#include "epoll/Epoll.hpp"
#include "requests/ResponseError.hpp"

class Logger;

class Loop
{
	private:
		std::map<std::string, Server>	_servers;
		std::vector<Socket *>			_sockets;
		std::vector<Client *>			_clients;
		Epoll							_epoll;
		std::string						_hostnameOfSrvSock;

		void	_sockOptNonBlocking(int &socketFd);

		void	_createMapServer(std::vector<Server> servers);

		void	_closeClients(int idClient);

		void	_checkAllTimeout();
		bool	_isServerSocket(int fd);
		bool	_isClientSocket(int fd, int &idClient);

		void	_acceptClient(int fd);

		bool	_parsingRequest(int idClient);
		int		_receiveRequest(int idClient);
		void	_addBodyLen(int idClient);
		void	_addBodyChunked(int idClient);
		void	_checkBody(int idClient);
		bool	_getRequest(int idClient);

		void	_createResponse(int idClient);
		void	_createTimeoutResponse(int idClient);
		void	_sendResponse(int idClient);
		
		void	_printTime();
		void	_printSocket();
		void	_printSend(int idClient);

	public:
		Loop(std::vector<Server> servers, std::vector<Socket *> sockets, int nbSockets);
		~Loop();

		void	runLoop();
};

#endif