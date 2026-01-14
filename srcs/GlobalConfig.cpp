#include "../includes/GlobalConfig.hpp"

// CONSTRUCTOR & DESTRUCTOR

GlobalConfig::GlobalConfig(std::string path)
{
	std::vector<struct server>	vectStructSrv;
	int							sizeVectStructSrv;
	
	vectStructSrv = createVectStructSrv(path);
	sizeVectStructSrv = vectStructSrv.size();
	for (int i = 0; i < sizeVectStructSrv; i++)
	{
		Server	tempSrv(vectStructSrv[i]);
		if (isDuplicateServer(tempSrv.getHostname()))
			throw std::invalid_argument(RED "Error : this hostname exists multiple times" RESET);
		this->_hostnames.push_back(tempSrv.getHostname());
		this->_allServers[tempSrv.getHostname()] = tempSrv;
	}
}

GlobalConfig::~GlobalConfig()
{
}

// METHODS
//PRIVATE

bool	GlobalConfig::isDuplicateServer(std::string hostnameToAdd)
{
	int	sizeHostname;

	sizeHostname = this->_hostnames.size();
	for (int i = 0; i < sizeHostname; i++)
	{
		if (this->_hostnames[i] == hostnameToAdd)
			return (true);
	}
	return (false);
}

// GETTERS

std::vector<std::string> const		&GlobalConfig::getHostnames() const
{
	return (this->_hostnames);
}

std::map<std::string, Server> const	&GlobalConfig::getAllServers() const
{
	return (this->_allServers);
}

std::vector<Socket *> const			&GlobalConfig::getServerSockets() const
{
	return (this->_serverSockets);
}
