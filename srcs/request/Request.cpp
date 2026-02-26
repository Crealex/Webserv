#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <sstream>

std::vector<std::string> Request::_v = Request::_acceptedType();

Request::Request() : _ContentLength(0), _keepAlive(false), _error(false)
{
}

Request::Request(const Request& cpy)
{
	_method = cpy.getMethod();
	_location = cpy.getLocation();
	_protocol = cpy.getProtocol();
	_host = cpy.getHost();
	_userAgent = cpy.getUserAgent();
	_accept = cpy.getAccept();
	_ContentType = cpy.getContentType();
	_ContentLength = cpy.getContentLength();
	_body = cpy.getBody();
	_keepAlive = cpy.getkeepAlive();
}

Request &Request::operator=(const Request& src)
{
	if (this != &src)
	{
		_method = src.getMethod();
		_location = src.getLocation();
		_protocol = src.getProtocol();
		_host = src.getHost();
		_userAgent = src.getUserAgent();
		_accept = src.getAccept();
		_ContentType = src.getContentType();
		_ContentLength = src.getContentLength();
		_body = src.getBody();
		_keepAlive = src.getkeepAlive();
	}
	return *this;
}

Request::~Request()
{
}

/**
 * @brief return a hard vector with the different accepted Content-Type
 * 
 * @return Vector with the Content-Type
 */
std::vector<std::string> Request::_acceptedType()
{
	std::vector<std::string> v;

	v.push_back("*/*");
	v.push_back("text/html");
	v.push_back("text/css");
	v.push_back("image/png");
	v.push_back("video/mp4");
	v.push_back("text/markdown");
	v.push_back("application/json");
	v.push_back("text/javascript");
	v.push_back("audio/mpeg");
	v.push_back("image/jpeg");
	v.push_back("text/csv");
	v.push_back("image/gif");
	v.push_back("audio/wav");
	v.push_back("application/zip");
	v.push_back("application/pdf");
	v.push_back("image/vnd.microsoft.icon");
	v.push_back("application/octet-stream");
	v.push_back("video/x-msvideo");
	v.push_back("image/avif");
	v.push_back("text/plain"); // INFO: ajouter par alex
	v.push_back("multipart/form-data"); // INFO: ajouté par alex

	return v;
}

void Request::reset()
{
	if (!_method.empty())
		_method.clear();
	if (!_location.empty())
		_location.clear();
	if (!_protocol.empty())
		_protocol.clear();
	if (!_host.empty())
		_host.clear();
	if (!_userAgent.empty())
		_userAgent.clear();
	if (!_accept.empty())
		_accept.clear();
	if (!_ContentType.empty())
		_ContentType.clear();
	if (!_body.empty())
		_body.clear();
	_bodySize = 0;
	_ContentLength = 0;
	_keepAlive = false;
}

void Request::_parseAccept()
{
	std::istringstream ss(_accept);
	std::string	newAccept;
	std::string	line;
	while (std::getline(ss, line, ','))
	{
		size_t pos = line.find(';');
		if (pos != std::string::npos)
			newAccept += line.substr(0, pos);
		else
			newAccept += line;
		newAccept += ',';
	}
	newAccept.erase(newAccept.end() - 1);
	_accept = newAccept;
}

void Request::_checkGet()
{
	if (_userAgent.empty() || 
		_accept.empty())
	{
		throw ResponseError(411, "Missing value", *this);
	}

	int leave = 0;
	for (std::vector<std::string>::iterator it = _v.begin();
	it != _v.end(); it++)
	{
		std::istringstream iss(_accept);
		std::string str;
		while (std::getline(iss, str, ','))
		{
			if (str.compare(*it) == 0)
			{
				leave = 1;
				break ;
			}
		}

		if (leave == 1)
			break ;

		if (it + 1 == _v.end())
			throw ResponseError(415, "Unsupported Media Type", *this);
	}
}

void Request::_checkPost()
{
	int leave = 0;
	if (_ContentType.empty() || 
		_body.empty())
	{
		throw ResponseError(411, "Missing value", *this);
	}

	for (std::vector<std::string>::iterator it = _v.begin(); // INFO: Toutes la boucle for complémentent modif (par alex avec l'aval de kiki)
		it != _v.end(); it++)
	{
		std::istringstream iss(_accept);
		std::string str;
		while (std::getline(iss, str, ','))
		{
			if (str.compare(*it) == 0)
			{
				leave = 1;
				break ;
			}
		}

		if (leave == 1)
			break ;

		if (it + 1 == _v.end())
			throw ResponseError(415, "Unsupported Media Type", *this);
	}
}

void Request::checkRequest(const unsigned int maxSize)
{
	if (_error)
	{
		throw ResponseError(400, "Bad request", *this);
	}	

	if (_method != "GET" &&
		_method != "POST" &&
		_method != "DELETE")
	{
		throw ResponseError(501, "Not implemented method", *this);
	}

	if (_protocol != "HTTP/1.1" &&
		_protocol != "HTTP/1.0")
	{
		throw ResponseError(505, "HTTP version not supported", *this);
	}

	if (_bodySize > maxSize)
	{
		throw ResponseError(413, "Content too large", *this);
	}

	if (_ContentLength && !_transferEncoding.empty())
	{
		throw ResponseError(400, "Bad request", *this);
	}

	_parseAccept();
	if (_method == "POST")
		_checkPost();
	else if (_method == "GET")
		_checkGet();
}

/**
 * @brief Parse the request header : The method, the location and the protocol
 * 
 * @param line 
 * @param req 
 */
void Request::setFirstLine(std::string &line)
{
	std::stringstream ss(line);
	std::string check;

	ss >> _method;
	ss >> _location;
	ss >>_protocol;
	if (ss >> check)
		_error = true;
}

/**
 * @brief Create a map with key and pointer to the right string in the Request object
 * 
 * @param req 
 * @return std::map<std::string, std::string*> 
 */
 std::map<std::string, std::string*> Request::createMap()
{
	std::map<std::string, std::string*> ret;

	ret.insert(std::make_pair(std::string("Host:"), &_host));
	ret.insert(std::make_pair(std::string("User-Agent:"), &_userAgent));
	ret.insert(std::make_pair(std::string("Accept:"), &_accept));
	ret.insert(std::make_pair(std::string("Content-Type:"), &_ContentType));
	ret.insert(std::make_pair(std::string("Transfer-Encoding:"), &_transferEncoding));

	return ret;
}

/**
 * @brief extract all the body properly and return it
 * 
 * @param str 
 * @return std::string 
 */
std::string Request::retBody(std::string &str)
{
	std::stringstream target(str);

	target.seekg(0, std::ios::end);
	_bodySize = target.tellg();
	target.seekg(0, std::ios::beg);
	char* buffer = new char[_bodySize];
	target.read(buffer, _bodySize);
	std::string file(buffer, _bodySize);
	delete[] buffer;

	return file;
}

/**
 * @brief Parse the client request and create a Request structure
 * 
 * @param buffer the client request
 * @return The created Request object
 */
void Request::parseHeader(std::string &buffer)
{
	std::map<std::string, std::string*> ptrMap = createMap();
	std::istringstream iss(buffer);
	std::string line;

	std::getline(iss, line);
	setFirstLine(line);
	while (std::getline(iss, line)) 
	{
		if (!line.empty() && line[line.size() - 1] == '\r') // INFO: Ajouter par Alex (pour gerer les \r)
        line.erase(line.size() - 1);
		// extract and parse the different element of the request
		if (line.empty())
			break ;

		std::stringstream ss(line);
		std::string word;
		ss >> word;
		if (word == "Content-Length:")
		{
			if (!(ss >> _ContentLength))
				_error = true;
			continue ;
		}
		if (word == "Connection:")
		{
			ss >> word;
			if (word == "keep-alive")
				_keepAlive = true;
			continue ;
		}
		try
		{
			std::string* strPtr = ptrMap.at(word);
			if (!strPtr->empty())
				_error = true;
			std::string str;
			while (ss >> word)
				str.append(word + ' ');
			str.erase(str.end() - 1);
			*strPtr = str;
		}
		catch(...)
		{
		}
	}

	_URI = _host + _location;
	size_t pos = _location.find_first_of('?', 0);
	_query = _location.substr(pos + 1);
}

void Request::parseBody(std::string &buffer)
{
	// std::cout << RED << "buffer = " << buffer << "pos = " << pos << RESET << std::endl;
	_body = retBody(buffer);
	std::cout << "body = " << _body << std::endl;
}

std::string	Request::getRawRequest() const
{
	std::stringstream rawSS;
	rawSS 
		<< "method = " << _method
		<< "\nlocation = " << _location
		<< "\nprotocol = " << _protocol
		<< "\nhost = " << _host
		<< "\nuserAgent = " << _userAgent
		<< "\naccept = " << _accept
		<< "\ncontent type = " << _ContentType
		<< "\ncontent length = " << _ContentLength
		<< "\nbody = " << _body << std::endl;
	return (rawSS.str());
}

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getLocation() const
{
	return _location;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

std::string Request::getHost() const
{
	return _host;
}

std::string Request::getUserAgent() const
{
	return _userAgent;
}

std::string Request::getAccept() const
{
	return _accept;
}

std::string Request::getTranferEncoding() const
{
	return _transferEncoding;
}

std::string Request::getContentType() const
{
	return _ContentType;
}

std::string Request::getURI() const
{
	return _URI;
}

std::string Request::getQuery() const
{
	return _query;
}

std::string Request::getBody() const
{
	return _body;
}

bool Request::getkeepAlive() const
{
	return _keepAlive;
}

unsigned int Request::getContentLength() const
{
	return _ContentLength;
}

std::string Request::getStrContentLength() const
{
	std::stringstream ss;
	std::string	str;
	ss << _ContentLength;
	ss >> str;
	return str;
}

void	Request::setkeepAlive(bool b)
{
	_keepAlive = b;
}

void	Request::setMethod(std::string str)
{
	_method = str;
}

void	Request::setLocation(std::string str)
{
	_location = str;
}

void	Request::setProtocol(std::string str)
{
	_protocol = str;
}

void	Request::setHost(std::string str)
{
	_host = str;
}

void	Request::setUserAgent(std::string str)
{
	_userAgent = str;
}

void	Request::setAccept(std::string str)
{
	_accept = str;
}

void Request::setTranferEncoding(std::string str)
{
	_transferEncoding = str;
}

void	Request::setContentType(std::string str)
{
	_ContentType = str;
}

void	Request::setBody(std::string str)
{
	_body = str;
}

void	Request::setContentLength(unsigned int n)
{
	_ContentLength = n;
}
