#include "../includes/Client.hpp"
#include "../includes/requests/Method.hpp"

// CONSTRUCTOR & DESTRUCTOR
Client::Client()
{
	this->_fdSocket = -1;
	this->_time = this->getTimeNow();
	this->_isSend = false;
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

	std::cout << MAGENTA;
	std::cout << "Close of client with fd : " << this->_fdSocket;
	std::cout << "\t" << display;
	std::cout << RESET << std::endl;
	if (this->_fdSocket > 0)
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

bool const			&Client::getIsSend() const
{
	return (this->_isSend);
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
	if (!this->_response.empty())
		this->_response.clear();
	printf("resp : %lu\n", this->_response.size());
	this->_response = str;
	printf("resp : %lu\n", this->_response.size());
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

void	Client::setIsSend(bool newIsSend)
{
	this->_isSend = newIsSend;
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
	
	timestamp = std::time(NULL);
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
	this->_isSend = false;
}

void	Client::checkRequest(Server server)
{
	this->_request.checkRequest(server.getMaxSize());
}

bool	Client::checkTimeoutRequest()
{
	std::time_t timeout;

	timeout = std::difftime(this->getTimeNow(), this->_timeRequest);
	if (timeout > MAXTIMEREQUEST)
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

void	Client::startCGI(Server &serv)
{
	_CGI.setEnvp(serv, _request);
	_CGI.constructFD();
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

	if (interpreter.empty())
	{
		std::string substring = _request.getLocation().substr(0, _request.getLocation().find_last_of('/'));
		std::string ext = _request.getLocation().substr(_request.getLocation().find('.'), _request.getLocation().size() - _request.getLocation().find('.'));
		for (size_t i = 0; i < serv.getLocations().size(); i++)
		{
			if (substring == serv.getLocations()[i].getPath())
			{
				interpreter = serv.getLocations()[i].getCgiHandler().at(ext);
			}
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
	else
		path = serv.getRoot() + path;
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
	try
	{
		_CGI.checkSubprocess(this->_request);
		
		double diff = std::difftime(std::time(NULL), _timeRequest);	
		// printf("diff = %f\n", diff);
		if (diff > MAXTIMEREQUEST)
		{
			throw ResponseError(408, "Request timeout", _request);
		}
	}
	catch (ResponseError &e)
	{
		_response = e.createResponse(serv);
		return true;
	}
	if (_CGI.subprocessExited())
	{
		_response = _CGI.getResponse();
		_checkCGIResponse();
		// std::cout << BOLD << RED << "CGI RESPONSE : \n" << RESET << _response << std::endl;
		return true;
	}
	return false;
}

void	Client::settingRequestStatus(enum statusType newStatus)
{
	this->_request.setStatus(newStatus);
}

void	Client::printAddPort()
{
	std::stringstream	port;

	port << ::ntohs(this->_sockadd.sin_port);
	std::cout << this->_intToIp();
	std::cout << ":";
	std::cout << port.str();
}
