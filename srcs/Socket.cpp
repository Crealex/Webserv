#include "../includes/Socket.hpp"
#include <sstream>
#include <string>
#include <sys/socket.h>

Socket::Socket(Config conf, int i)
{
	assignmentSocket(conf, i);
}

Socket::~Socket()
{
	size_t	nbFds;

	nbFds = this->_fdServer.size();
	for (int i = 0; i < nbFds; i++)
	{
		if (this->_fdServer[i])
			close(this->_fdServer[i]);
	}
}

// GETTERS
std::string	Socket::getHostname() const
{
	return (this->_hostname);
}

std::vector<sockaddr_in>	Socket::getSockaddrs() const
{
	return (this->_sockaddrs);
}

std::vector<int>	Socket::getFdServer() const
{
	return (this->_fdServer);
}

std::vector<int>	Socket::getFdClient() const
{
	return (this->_fdClient);
}

// SETTER
void	Socket::setFdClient()
{

}

// METHODS
static uint32_t ipToUint(std::string s)
{
	std::stringstream			ss;
	std::stringstream			toBytes;
	unsigned int				bytesTemp;
	std::string					temp;
	std::vector<unsigned int>	bytes;
	uint32_t					result;

	bytes.reserve(4);
	ss << s;
	while (std::getline(ss, temp, '.'))
	{
		toBytes << temp;
		toBytes >> bytesTemp;
		bytes.push_back(bytesTemp);
	}
	result = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
	return (result);
}

void Socket::assignmentSocket(Config conf, int i)
{
	size_t	nbAddPort;
	int		socketFd;

	this->_hostname = (conf.getAddressPort()[i].name);

	nbAddPort = conf.getAddressPort()[i].addressPort.size();
	this->_sockaddrs.reserve(nbAddPort);
	this->_fdServer.reserve(nbAddPort);
	this->_fdClient.reserve(nbAddPort);
	for (size_t index = 0; index < nbAddPort; index++)
	{
		sockaddr_in	soAdd;
		soAdd.sin_family = AF_INET;
		soAdd.sin_port = htons(conf.getAddressPort()[i].addressPort[index].second);
		soAdd.sin_addr.s_addr = ipToUint(conf.getAddressPort()[i].addressPort[index].first);
		this->_sockaddrs.push_back(soAdd);
		socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketFd != -1)
		{
			sockOpt(socketFd);
			this->_fdServer.push_back(socketFd);
		}
	}
}

void	Socket::sockOpt(int &socketFd)
{
		int	opt;

		opt = 1;
		setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}
