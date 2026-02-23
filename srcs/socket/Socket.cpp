#include "../../includes/socket/Socket.hpp"

Socket::Socket(Server data)
{
	this->_hostname = (data.getHostname());
	addingSockets(data);
}

Socket::~Socket()
{
	int	sizeSockData;

	sizeSockData = this->_sockData.size();
	for (int i = 0; i < sizeSockData; i++)
	{
		if (!this->_sockData[i])
		{
			delete this->_sockData[i];
			this->_sockData[i] = NULL;
		}
	}
	this->_sockData.clear();
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

// METHODS
// PRIVATE
void	Socket::addingSockets(Server data)
{
	size_t		nbAddPort;

	nbAddPort = data.getAddressPort().size();
	for (size_t index = 0; index < nbAddPort; index++)
	{
		SocketData	*result = new SocketData(data.getAddressPort()[index]);
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
	delete this->_sockData[i];
	this->_sockData[i] = NULL;
	this->_sockData.erase(this->getBegin() + i);
}

void	Socket::printSockData()
{
	int	sizeSockData;

	sizeSockData = this->_sockData.size();
	for (int i = 0; i < sizeSockData; i++)
	{
		this->_sockData[i]->printAddrPort();
	}
}