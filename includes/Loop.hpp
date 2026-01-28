#ifndef LOOP_HPP

#define LOOP_HPP

#include "includes.hpp"
#include "socket/includeSend.hpp"
#include "socket/Socket.hpp"

class Loop
{
	private:
		std::map<std::string, Server>	_servers;
		std::vector<Socket *>			_sockets;
		std::vector<Client *>			_clients;
		Epoll							_epoll;
		std::string						_hostnameOfSrvSock;

		void	_createMapServer(std::vector<Server> servers);
		void	_closeClients(int idClient);
		void	_checkAllTimeout();
		bool	_isServerSocket(int fd);
		bool	_isClientSocket(int fd, int &idClient);
		int		_acceptClient(int fd);
		int		_receiveRequest(int idClient);

	public:
		Loop(std::vector<Server> servers, std::vector<Socket *> sockets, int nbSockets);
		~Loop();

		void	runLoop();
};

#endif