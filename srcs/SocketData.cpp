#include "../includes/SocketData.hpp"
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
	std::stringstream			toBytes;
	std::vector<unsigned int>	bytes;
	uint32_t					result;

	bytes.reserve(4);
	if (std::sscanf(s.c_str(), "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) == 4)
	{
		if (bytes[0] >= 256 || bytes[1] >= 256 || bytes[2] >= 256 || bytes[3] >= 256)
			throw std::invalid_argument(RED "Error : bytes not right in IP address : higher than 256" RESET);
	}
	else
		throw std::invalid_argument(RED "Error : bytes not right in IP address" RESET);
	result = (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + bytes[0];
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

// METHODS
void	SocketData::assignmentSocket(addPort_t addPort)
{
	int	socketFd;

	this->_sockadd.sin_family = AF_INET;
	this->_sockadd.sin_port = htons(addPort.second);
	std::cout << "port : " << this->_sockadd.sin_port << std::endl;
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