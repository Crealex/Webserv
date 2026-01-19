#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"

/**
 * @brief create the right Method object and return it in a Methods pointer
 * 
 * @param buffer the client request
 * @return Methods* 
 */
Methods* createMethod(char *buffer, size_t maxSize, bool &ret)
{
	std::string method;
	Request req = createRequest(buffer, maxSize);

	ret = req._keepAlive;
	if (req._method == "GET")
		return new Get(req);
	else if (req._method == "POST")
		return new Post(req);
	else if (req._method == "DELETE")
		return new Delete(req);
	else
		throw ResponseError(501, "Error: Not implemented method", req);
}
