#include "../../includes/socket/Socket.hpp"

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

std::vector<SocketData *> const	&Socket::getSockData() const
{
	return (this->_sockData);
}

// SETTER
void	Socket::setFdClient(int newFdClient, size_t indexSocketData)
{
	this->_sockData[indexSocketData]->setFdClient(newFdClient);
}

// METHODS
// PRIVATE
void	Socket::addingSockets(serverData data)
{
	size_t		nbAddPort;

	nbAddPort = data.addressPort.size();
	for (size_t index = 0; index < nbAddPort; index++)
	{
		SocketData	*result = new SocketData(data.addressPort[index]);
		this->_sockData.push_back(result);
	}
}

// PUBLIC
std::vector<SocketData *>::iterator	Socket::getBegin()
{
	return (this->_sockData.begin());
}

std::vector<SocketData *>::iterator	Socket::getEnd()
{
	return (this->_sockData.end());
}

void	Socket::eraseSocket(int i)
{
	this->_sockData.erase(this->getBegin() + i);
}