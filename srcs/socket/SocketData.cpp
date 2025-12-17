#include "../../includes/socket/SocketData.hpp"
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>

SocketData::SocketData(addPort_t addPort)
{
	assignmentSocket(addPort);
}

SocketData::~SocketData()
{
	if (this->_fdServer)
		close(this->_fdServer);
}

static uint32_t ipToUint(std::string s)
{
	std::stringstream	toBytes;
	unsigned int		byte0;
	unsigned int		byte1;
	unsigned int		byte2;
	unsigned int		byte3;
	uint32_t			result;

	if (std::sscanf(s.c_str(), "%u.%u.%u.%u", &byte0, &byte1, &byte2, &byte3) == 4)
	{
		if (byte0 >= 256 || byte1 >= 256 || byte2 >= 256 || byte3 >= 256)
			throw std::invalid_argument(RED "Error : bytes not right in IP address : higher than 256" RESET);
	}
	else
		throw std::invalid_argument(RED "Error : bytes not right in IP address" RESET);
	result = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
	return (result);
}

//GETTERS
sockaddr_in const	&SocketData::getSockadd() const
{
	return (this->_sockadd);
}

int const	&SocketData::getFdServer() const
{
	return (this->_fdServer);
}

int const	&SocketData::getFdClient() const
{
	return (this->_fdClient);
}

//SETTERS
void	SocketData::setFdClient(int newFdClient)
{
	this->_fdClient = newFdClient;
}

// METHODS
void	SocketData::assignmentSocket(addPort_t addPort)
{
	int	socketFd;

	this->_sockadd.sin_family = AF_INET;
	this->_sockadd.sin_port = htons(addPort.second);
	this->_sockadd.sin_addr.s_addr = ipToUint(addPort.first);
	socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFd != -1)
	{
		sockOpt(socketFd);
		this->_fdServer = (socketFd);
	}
}
void	SocketData::sockOpt(int &socketFd)
{
		int	opt;

		opt = 1;
		setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}
