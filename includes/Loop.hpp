#ifndef LOOP_HPP

#define LOOP_HPP

#include "includes.hpp"
#include "epoll/Epoll.hpp"
#include "requests/ResponseError.hpp"
#include "Logger.hpp"

#define SOCKET true
#define SEND true

class	Logger;
void	signalHandler(int signal);

class Loop
{
	private:

		std::map<std::string, Server>	_servers;
		std::vector<Socket *>			_sockets;
		std::vector<Client *>			_clients;
		Epoll							_epoll;
		std::string						_hostnameOfSrvSock;
		bool							_isExit;
		
		void	_sockOptNonBlocking(int &socketFd);
		void	_createMapServer(std::vector<Server> servers);
		void	_listenSocket(size_t &i, size_t &j, size_t &sizeSockData);
		void	_bindSocket();
		void	_createSocket(std::vector<Server> srvs);
		
		void	_displayHelp();
		void	_handleCmd();

		void	_closeClients(int idClient);

		void	_checkAllTimeout();
		bool	_isServerSocket(int fd);
		bool	_isClientSocket(int fd, int &idClient);

		void	_acceptClient(int fd);

		void	_parsingRequest(int idClient, size_t posCRLF);
		int		_receive(int idClient);
		void	_addBodyLen(int idClient);
		void	_addBodyChunked(int idClient, size_t &posCRLF);
		bool	_checkBody(int idClient);
		bool	_receiveRequest(int idClient);

		void	_createResponse(int idClient);
		void	_createTimeoutResponse(int idClient);
		void	_sendResponse(int idClient);
		bool	_isCGI(int fd, int &idClient);
		
		void	_printSocket();
		void	_printSend(int idClient);

	public:
		Loop(std::vector<Server> servers);
		~Loop();

		void	runLoop();
};

#endif