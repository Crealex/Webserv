#include "../includes/SocketData.hpp"
#include <sstream>
#include <string>
#include <sys/socket.h>

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