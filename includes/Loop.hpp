#ifndef LOOP_HPP

#define LOOP_HPP

#include "includes.hpp"
#include "Client.hpp"
#include "socket/Socket.hpp"
#include "epoll/Epoll.hpp"

class Loop
{
	private:
		std::vector<Server>		_servers;
		std::vector<Socket *>	_sockets;
		std::vector<Client *>	_clients;

	public:
		Loop();
		~Loop();

		void	runLoop();
};

#endif