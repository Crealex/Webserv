#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <fstream>

Get::Get(Request requ): Methods(requ), _accept(requ._accept), _userAgent(requ._userAgent)
{
    std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}

// *** Create response

static std::string sendError(unsigned int code)
{
	(void)code;
	return ("Heu y a un bleme ma guele");
}

static bool addContentType(std::string *resp, std::string accept)
{
	std::string contentType;

	// detecte le type de contenu demandé
	if (contentType != accept && accept != "*/*")
		return (false);
	return (true);
}

const std::string Get::createResponse()
{
	std::string resp;
	std::ifstream target;
	
	if (!addContentType(&resp, this->_accept))
		return (sendError(666));
	target.open(this->_host + this->_location);
	if (!target.is_open())
		return (sendError(666));
	

	return (resp);
}

//*** RESPONSE EXAMPLE ***
//	HTTP/1.1 200 OK
//	Content-Type: text/html; charset=UTF-8
//	Date: Fri, 21 Jun 2024 14:18:33 GMT
//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
//	Content-Length: 1234
//	
//	<!doctype html>
//	<!-- Contenu HTML -->
