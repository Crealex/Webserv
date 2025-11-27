#include "../includes/Socket.hpp"

Socket::Socket(Config conf, int i)
{
	this->_hostname = (conf.getAddressPort()[i].name);
	size_t	nbAddPort;

	nbAddPort = conf.getAddressPort()[i].addressPort.size();
	for (int index = 0; index < nbAddPort; index++)
	{
		sockaddr_in	soAdd;
		soAdd.sin_family = AF_INET;
		soAdd.sin_port = htons(conf.getAddressPort()[i].addressPort[index].second);
		soAdd.sin_addr.s_addr = conf.getAddressPort()[i].addressPort[index].first;
		this->_sockaddrs.push_back(soAdd);
	}
	this->_sockaddrs = 
}

Socket::~Socket()
{
	size_t	nbFds;

	nbFds = this->_fds.size();
	for (int i = 0; i < nbFds; i++)
	{

	}
}