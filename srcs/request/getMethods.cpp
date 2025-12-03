#include "../../includes/requests/Request.hpp"
#include "request.cpp"

static Get* createGet(Request req)
{
	
}

static Post* createPost(Request req)
{
	
}

static Delete* createDelete(Request req)
{

}

Methods* createMethod(char *buffer)
{
	Methods* ret;
	std::string method;
	Request req = createRequest(buffer);

	if (req._method == "GET")
		return createGet(req);
	else if (req._method == "POST")
		return createPost(req);
	else if (req._method == "DELETE")
		return createDelete(req);
	// else
	//	throw wrong method error

}
