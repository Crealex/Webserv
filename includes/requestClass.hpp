#ifndef REQUEST_CLASS_HPP

# define REQUEST_CLASS_HPP
# include "includes.hpp"

class Request {
private:
	std::string		_method;
	std::string		_location;
	std::string		_protocol;
	std::string		_host;
	// decoupe dans plusieurs class enfant
	std::string		_userAgent;
	std::string		_accept;
	std::string		_ContentType;
	unsigned int	_ContentLength;
	std::string		_body;
	Request();
public:
	virtual const std::string createResponse() = 0;

	
};
#endif // !REQUEST_CLASS_HPP

