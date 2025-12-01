#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <fstream>
#include <string>
#include <sys/stat.h>

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
	
	//TODO: Add string message for addStartLine
	target.open(this->_host + this->_location);
	if (!target.is_open())
		return (sendError(666));
	stat((this->_host + this->_location).c_str(), &fileData);
	std::getline(target, file, '\0');
	if (!addContentType(&resp, this->_accept, this->_location)) // maked, need to test
		return (sendError(666));
	if (!addDate(&resp))
		return (sendError(666));
	if (!addLastModif(&resp, this->_host + this->_location))
		return (sendError(666));
	if (!addContentLenght(&resp, file))
		return (sendError(666));
	if (!addBody(&resp, file))
		return (sendError(666));
	if (!addStartLine(&resp, this->_protocol, 200))
		return (sendError(666));

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
	Get		resp(requ);
	std::cout << "resp: " << std::endl;
	std::cout << resp.createResponse() << std::endl;

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
