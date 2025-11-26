#include "../../includes/requests/methodsClass.hpp"
#include "../../includes/requests/Request.hpp"

Methods::Methods(Request requ): _location(requ._location), _protocol(requ._protocol), _host(requ._host), _method(requ._method)
{
}
