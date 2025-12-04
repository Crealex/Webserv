#include "../includes/Socket.hpp"

Socket::Socket(serverData data)
{
	this->_hostname = (data.name);
	addingSockets(data);
}

Socket::~Socket()
{
}

// GETTERS
std::string const	&Socket::getHostname() const
{
	return (this->_hostname);
}

std::vector<SocketData>	Socket::getSockData()
{
	return (this->_sockData);
}

std::vector<SocketData> const	&Socket::getSockData() const
{
	return (this->_sockData);
}

// SETTER
void	Socket::setFdClient()
{

}

// METHODS
void	Socket::addingSockets(serverData data)
{
	size_t		nbAddPort;

	nbAddPort = data.addressPort.size();
	for (size_t index = 0; index < nbAddPort; index++)
	{
		SocketData	result(data.addressPort[index]);
		this->_sockData.push_back(result);
	}
}
