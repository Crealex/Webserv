#include "../includes/Client.hpp"

// CONSTRUCTOR & DESTRUCTOR
Client::Client()
{
	this->_fdSocket = -1;
	this->_endOfFile = false;
	this->_keepAlive = true;
}

Client::~Client()
{
}

// GETTERS
std::string const	&Client::getHostname() const
{
	return (this->_hostname);
}

int const	&Client::getFdClient() const
{
	return (this->_fdSocket);
}

std::string const	&Client::getBuf() const
{
	return (this->_buf);
}

sockaddr_in const	&Client::getSockadd() const
{
	return (this->_sockadd);
}

bool const	&Client::getEndOfFile() const
{
	return (this->_endOfFile);
}

// SETTERS
void	Client::setHostname(std::string newHostname)
{
	this->_hostname = newHostname;
}

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

void	Client::setEndOfFile(bool newEndOfFile)
{
	this->_endOfFile = newEndOfFile;
}
