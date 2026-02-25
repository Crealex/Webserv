#include "../includes/Client.hpp"
#include "../includes/requests/Method.hpp"

// CONSTRUCTOR & DESTRUCTOR
Client::Client()
{
	this->_fdSocket = -1;
	this->_time = this->getTimeNow();
}

Client::~Client()
{
	std::time_t timeNow;
	struct tm	*timeDisplay;
	char		display[100];

	std::time(&timeNow);
	timeDisplay = std::localtime(&timeNow);
	std::strftime(display, sizeof(display), "Date: %a, %d.%m.%Y - %X", timeDisplay);
	_CGI.reset();

	std::cout << RED;
	std::cout << "Close of client with fd : " << this->_fdSocket;
	std::cout << "\t" << display;
	std::cout << RESET << std::endl;
	close (this->_fdSocket);
}

// GETTERS
bool const &Client::getIsCGI() const
{
	return (this->_isCGI);
}

std::string const	&Client::getHostname() const
{
	return (this->_hostname);
}

int const &Client::getFDCGI() const
{
	return _CGI.getReadFD();
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

void	Client::setTimeoutRequest()
{
	this->_timeRequest = this->getTimeNow();
}

void	Client::setTimeout()
{
	this->_time = this->getTimeNow();
}


//METHODS

// PRIVATE
std::string	Client::_intToIp()
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


// PUBLIC
std::time_t	Client::getTimeNow()
{
	std::time_t	timestamp;
	
	std::time(&timestamp);
	return (timestamp);
}

void	Client::resetBuf()
{
	this->_buf.clear();
}

void	Client::resetClient()
{
	if (!this->_buf.empty())
		this->_buf.clear();
	this->_request.reset();
	this->_timeRequest = this->getTimeNow();
	this->_time = this->getTimeNow();
	this->_isCGI = false;
	this->_CGI.reset();
	if (!this->_response.empty())
		this->_response.clear();
}

void	Client::checkRequest(Server server)
{
	this->_request.checkRequest(server.getMaxSize());
}

bool	Client::checkTimeoutRequest()
{
	if (std::difftime(this->getTimeNow(), this->_timeRequest) > MAXTIMEREQUEST)
		return (true);
	return (false);
}

bool	Client::checkTimeout()
{
	if (std::difftime(this->getTimeNow(), this->_time) > MAXTIME)
		return (true);
	return (false);
}

void Client::isCGI(Server &serv)
{
	_isCGI = _CGI.isCGI(_request.getLocation(), serv);
}

void	Client::startCGI(Server &serv, Epoll &epoll)
{
	_CGI.setEnvp(serv, _request);
	_CGI.constructFD(epoll);
	std::string interpreter;
	std::string path;

	for (size_t i = 0; i < serv.getLocations().size(); i++)
	{
		if (_request.getLocation() == serv.getLocations()[i].getPath())
		{
			std::string index = serv.getLocations()[i].getIndex();
			int pos = index.find('.');
			index.erase(0, pos);
			interpreter = serv.getLocations()[i].getCgiHandler().at(index);
		}
	}

	path = _request.getLocation();

	if (path.find('.') == std::string::npos)
	{
		for (size_t i = 0; i < serv.getLocations().size(); i++)
		{
			if (serv.getLocations()[i].getPath() == path)
			{
				path = serv.getRoot() + _request.getLocation() + '/' + serv.getLocations()[i].getIndex();
				break ;
			}
		}
	}
	_CGI.startSubprocess(path, interpreter);
	_CGI.sendBody(_request.getBody());
}

int	Client::_getSizeBody()
{
	int pos = _response.find("\r\n\r\n");
	pos += 4;
	int size = _response.size() - pos;

	return size;
}

void	Client::_checkCGIResponse()
{
	_response.append("\r\n\r\n");
	int ContentLength = _getSizeBody();
	std::string head;
	addStartLine(&head, "HTTP/1.1", 200, "OK");
	addDate(&head);
	addContentLenght(&head, ContentLength);
	_response = head + _response;
}

bool	Client::checkCGI(Server &serv)
{
	// std::cout << MAGENTA << BOLD << "checking CGI" << RESET << std::endl;
	try
	{
		_CGI.checkSubprocess(this->_request);
	}
	catch (ResponseError &e)
	{
		_response = e.createResponse(serv);
		::send(_fdSocket, _response.c_str(), _response.size(), 0);
		_CGI.reset();
		return true;
	}
	if (_CGI.subprocessExited())
	{
		_response = _CGI.getResponse();
		_checkCGIResponse();
		std::cout << BOLD << RED << "CGI RESPONSE : \n" << RESET << _response << std::endl;
		if (::send(_fdSocket, _response.c_str(), _response.size(), 0) == -1)
			throw std::runtime_error("Error sending response");
		_CGI.reset();
		return true;
	}
	return false;
}

void	Client::printAddPort()
{
	std::stringstream	port;

	port << ::ntohs(this->_sockadd.sin_port);
	std::cout << this->_intToIp();
	std::cout << ":";
	std::cout << port.str();
}
