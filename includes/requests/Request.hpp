#ifndef REQUEST_HPP
# define REQUEST_HPP

struct Request
{
	std::string		_method;
	std::string		_location;
	std::string		_protocol;
	std::string		_host;
	std::string		_userAgent;
	std::string		_accept;
	std::string		_ContentType;
	unsigned int	_ContentLength;
	std::string		_body;
}

#endif