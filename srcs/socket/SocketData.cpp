#include "../../includes/socket/SocketData.hpp"

SocketData::SocketData(addPort_t addPort)
{
	this->_assignmentSocket(addPort);
}

SocketData::~SocketData()
{
	if (this->_fdServer)
		close(this->_fdServer);
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

// METHODS
// PRIVATE

void	SocketData::_sockOptNonBlocking(int &socketFd)
{
		int	opt;

		opt = 1;
		::fcntl(socketFd, F_SETFL, O_NONBLOCK);
		::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}

uint32_t SocketData::_ipToUint(std::string s)
{
	std::stringstream	toBytes;
	unsigned int		byte0;
	unsigned int		byte1;
	unsigned int		byte2;
	unsigned int		byte3;
	uint32_t			result;
	
	std::sscanf(s.c_str(), "%u.%u.%u.%u", &byte0, &byte1, &byte2, &byte3);
	// {
	// 	if (byte0 >= 256 || byte1 >= 256 || byte2 >= 256 || byte3 >= 256)
	// 		throw std::invalid_argument(RED "Error : bytes not right in IP address : higher than 256" RESET);
	// }
	// else
	// 	throw std::invalid_argument(RED "Error : bytes not right in IP address" RESET);
	result = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
	return (result);
}

std::string	SocketData::_intToIp()
{
	std::stringstream	toStr;
	unsigned int		byte0;
	unsigned int		byte1;
	unsigned int		byte2;
	unsigned int		byte3;
	std::string			res;
	uint32_t			val = this->_sockadd.sin_addr.s_addr;

	byte0 = val & 0xFF;
	byte1 = (val >> 8) & 0xFF;
	byte2 = (val >> 16) & 0xFF;
	byte3 = (val >> 24) & 0xFF;
	toStr << byte0 << "." << byte1 << "." << byte2 << "." << byte3; 
	res = toStr.str();
	return (res);
}	

void	SocketData::_assignmentSocket(addPort_t addPort)
{
	int	socketFd;
	
	this->_sockadd.sin_family = AF_INET;
	this->_sockadd.sin_port = ::htons(addPort.second);
	this->_sockadd.sin_addr.s_addr = this->_ipToUint(addPort.first);
	socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFd != -1)
	{
		this->_sockOptNonBlocking(socketFd);
		this->_fdServer = (socketFd);
	}
}	

// PUBLIC
void	SocketData::printAddrPort()
{
	std::cout << this->_intToIp();
	std::cout << ":";
	std::cout << ::ntohs(this->_sockadd.sin_port);
}	
