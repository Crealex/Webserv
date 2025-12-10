
#include "../../includes/requests/Delete.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include "../../includes/requests/methodsClass.hpp"
#include <unistd.h>

Delete::Delete(Request requ) : Methods(requ)
{
	std::cout << GREEN << "Default Delete constructor called" << RESET << std::endl;
}

const std::string Delete::createResponse()
{
	std::string path;
	std::string resp;
	Request dataError;

	path = this->_host + this->_location;
	if (access(path.c_str(), F_OK) == -1)
	{
		if (!addStartLine(&resp, this->_protocol, 202, "no content"))
			throw(ResponseError(500, "can't add start line", dataError));
		if (!addDate(&resp))
			throw(ResponseError(500, "can't add date", dataError));
		resp.append("Server: webserv");
	}
	return (resp);
}
