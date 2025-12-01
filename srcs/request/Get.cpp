#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "../../includes/requests/ResponseError.hpp"

Get::Get(Request requ): Methods(requ), _accept(requ._accept), _userAgent(requ._userAgent)
{
    std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}

// *** Create response

//INFO: Juste pour avoir une fonctions mais sera porbablement pas comme ça
static std::string sendError(unsigned int code)
{
	(void)code;
	return ("Heu y a un bleme ma guele");
}

const std::string Get::createResponse()
{
	std::string		resp;
	std::ifstream	target;
	std::string		file;
	struct stat		fileData;
	Request			dataError;
	
	dataError._protocol = this->_protocol;
	dataError._host = this->_host;
	dataError._accept = this->_accept;
	dataError._location = this->_location;
	dataError._userAgent = this->_userAgent;
	target.open(this->_host + this->_location);
	if (!target.is_open())
		throw ResponseError(404, "Not found", dataError);
	stat((this->_host + this->_location).c_str(), &fileData);
	std::getline(target, file, '\0');
	if (!addContentType(&resp, this->_accept, this->_location))
		throw ResponseError(404, "Not found", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Internal server error", dataError);
	if (!addLastModif(&resp, this->_host + this->_location))
		throw ResponseError(500, "Internal server error", dataError);
	if (!addContentLenght(&resp, file))
		throw ResponseError(500, "Internal server error", dataError);
	if (!addBody(&resp, file))
		throw ResponseError(500, "Internal server error", dataError);
	if (!addStartLine(&resp, this->_protocol, 200, "OK"))
		throw ResponseError(500, "Internal server error", dataError);

	return (resp);
}


// *** TEST MAIN ***

int main(void)
{
	Request requ;

	requ._accept = "text/html";
	requ._host = "../../www";
	requ._location = "/demo/index.html";
	requ._protocol = "HTTP/1.1";
	requ._userAgent = "Firefox";
	try 
	{
		Get		resp(requ);
		std::cout << "resp: " << std::endl;
		std::cout << resp.createResponse() << std::endl;

	}
	catch (ResponseError &e)
	{
		std::cout << e.createResponse() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

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
