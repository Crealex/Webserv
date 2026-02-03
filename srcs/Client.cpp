#include "../includes/Client.hpp"

// CONSTRUCTOR & DESTRUCTOR
Client::Client()
{
	this->_fdSocket = -1;
	this->_keepAlive = false;
	this->_time = getTimeNow();
	this->_timeoutRequest = false;
}

Client::~Client()
{
	std::cout << RED << "IN THE DESTRUCTOR OF CLIENT : " << this->_fdSocket << std::endl << RESET;
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

Request const &Client::getRequest() const
{
	return (this->_request);
}

std::string const	&Client::getResponse() const
{
	return (this->_response);
}

bool const			&Client::getKeepAlive() const
{
	return (this->_keepAlive);
}	


bool const	&Client::getTimeoutRequest() const
{
	return (this->_timeoutRequest);
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

void	Client::setBuf(const char *newBuf, int size)
{
	if (this->_buf.empty())
		this->_buf = std::string(newBuf, size);
	else
		this->_buf.append(newBuf, size);
}

void	Client::setSockadd(sockaddr_in newSockadd)
{
	this->_sockadd = newSockadd;
}

void	Client::setResponse(const std::string &str)
{
	this->_response = str;
}

void	Client::setRequestHeader(std::string &str)
{
	this->_request.parseHeader(str);
}

void	Client::setRequestBody()
{
	this->_request.parseBody(this->_buf);
}

void	Client::setKeepAlive(bool newKeepAlive)
{
	this->_keepAlive = newKeepAlive;
}

void	Client::setTimeoutRequest()
{
	this->_timeRequest = getTimeNow();
}

void	Client::setTimeout()
{
	this->_time = getTimeNow();
}


//METHODS

void	Client::resetBuf()
{
	this->_buf.clear();
}

void	Client::resetClient()
{
	std::cout << GREEN << "in reset : " << this->_fdSocket << std::endl << RESET;
	this->_buf.clear();
	this->_request.reset();
	this->_keepAlive = false;
	this->_timeRequest = getTimeNow();
	this->_timeoutRequest = false;
}

void	Client::checkRequest(Server server)
{
	this->_request.checkRequest(server.getMaxSize());
}

void	Client::checkTimeoutRequest()
{
	if (std::difftime(getTimeNow(), this->_timeRequest) > MAXTIMEREQUEST)
		this->_timeoutRequest = true;
}

bool	Client::checkTimeout()
{
	if (std::difftime(getTimeNow(), this->_time) > MAXTIME)
		return (true);
	return (false);
}
