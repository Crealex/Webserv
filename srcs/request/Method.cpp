#include "../../includes/requests/Method.hpp"
#include "../../includes/requests/Request.hpp"

Methods::Methods(Request requ) : _method(requ.getMethod()), _location(requ.getLocation()), _protocol(requ.getProtocol()), _host(requ.getHost())
{
}

Request Methods::_createDataError()
{
	Request ret;

	ret.setProtocol(this->_protocol);
	ret.setHost(this->_host);
	ret.setLocation(this->_location);
	ret.setMethod(this->_method);
	return (ret);
}

Methods::~Methods()
{
	
}
