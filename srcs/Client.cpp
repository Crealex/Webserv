#include "../includes/Client.hpp"

// CONSTRUCTOR & DESTRUCTOR
Client::Client()
{
	this->_fdSocket = -1;
	this->_endOfFile = false;
}

Client::~Client()
{
}

// GETTERS
int const		Client::getFdClient() const
{
	return (this->_fdSocket);
}

std::string const	Client::getBuf() const
{
	return (this->_buf);
}

sockaddr_in const	Client::getSockadd() const
{
	return (this->_sockadd);
}

// SETTERS
void	Client::setFdClient(int newFd)
{
	this->_fdSocket = newFd;
}

void	Client::setBuf(char *newBuf)
{
	if (this->_buf.empty())
		this->_buf = newBuf;
	else
		this->_buf.append(newBuf);
}

void	Client::setSockadd(sockaddr_in newSockadd)
{
	this->_sockadd = newSockadd;
}
