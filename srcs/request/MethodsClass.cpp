#include "../../includes/requests/methodsClass.hpp"
#include "../../includes/requests/Request.hpp"

Methods::Methods(Request requ) : _method(requ.getMethod()), _location(requ.getLocation()), _protocol(requ.getProtocol()), _host(requ.getHost())
{
}

Methods::~Methods()
{
	
}