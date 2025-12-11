#include "../../includes/requests/Request.hpp"
#include "request.cpp"

/**
 * @brief create the right Method object and return it in a Methods pointer
 * 
 * @param buffer the client request
 * @return Methods* 
 */
Methods* createMethod(char *buffer)
{
	Methods* ret;
	std::string method;
	Request req = createRequest(buffer);

	if (req._method == "GET")
		return new Get(req);
	else if (req._method == "POST")
		return new Post(req);
	else if (req._method == "DELETE")
		return new Delete(req);
	// else
	//	throw wrong method error
}
