#ifndef GLOBALCONFIG_HPP

#define GLOBALCONFIG_HPP

#include "includes.hpp"
#include "Server.hpp"
#include "socket/Socket.hpp"

class GlobalConfig
{
	private:
		std::vector<std::string>		_hostnames;
		std::map<std::string, Server>	_allServers;
		std::vector<Socket *>			_serverSockets;

		bool	isDuplicateServer(std::string hostnameToAdd);

	public:
		GlobalConfig(std::string path);
		~GlobalConfig();

		std::vector<std::string> const		&getHostnames() const;
		std::map<std::string, Server> const	&getAllServers() const;
		std::vector<Socket *> const			&getServerSockets() const;
};

#endif