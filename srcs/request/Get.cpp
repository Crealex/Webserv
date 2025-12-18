#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "../../includes/requests/ResponseError.hpp"

Get::Get(Request requ): Methods(requ), _userAgent(requ._userAgent), _accept(requ._accept) 
{
    std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}

// *** Create response

const std::string Get::createResponse(Config conf)
{
	std::string		resp;
	std::ifstream	target;
	std::string		file;
	std::string		path;
	Request			dataError;
	
	dataError._protocol = this->_protocol;
	dataError._host = this->_host;
	dataError._accept = this->_accept;
	dataError._location = this->_location;
	dataError._userAgent = this->_userAgent;
	// root + sitename + (si pas fichier precis) defaultfile
	path = conf.getDirRoot(conf.getSitesName()[0]) + conf.getSitesName()[0] + this->_location;
	if (path.find(".") > path.length())
		path.append(conf.getDefaultFile(conf.getSitesName()[0]));
	std::cout << "raw path:" << path << std::endl;
	target.open(path.c_str());
	// *************************
	if (!target.is_open())
		throw ResponseError(404, "Not found", dataError);
	std::getline(target, file, '\0');
	if (!addContentType(&resp, this->_accept, path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, file))
		throw ResponseError(500, "can't add content lenght", dataError);
	if (!addBody(&resp, file))
		throw ResponseError(500, "can't add body", dataError);
	if (!addStartLine(&resp, this->_protocol, 200, "OK"))
		throw ResponseError(500, "can't add start line", dataError);

	return (resp);
}


// *** TEST MAIN ***

//int main(void)
//{
//	Request requ;
//
//	requ._accept = "text/html";
//	requ._host = "../../www";
//	requ._location = "/demo/index.html";
//	requ._protocol = "HTTe/1.1";
//	requ._userAgent = "Firefox";
//	try 
//	{
//		Get		resp(requ);
//		std::cout << "resp: " << std::endl;
//		std::cout << resp.createResponse() << std::endl;
//
//	}
//	catch (ResponseError &e)
//	{
//		std::cout << e.createResponse() << std::endl;
//	}
//	catch (std::exception &e)
//	{
//		std::cout << e.what() << std::endl;
//	}
//
//}

//*** RESPONSE EXAMPLE ***
//	HTTP/1.1 200 OK
//	Content-Type: text/html; charset=UTF-8
//	Date: Fri, 21 Jun 2024 14:18:33 GMT
//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
//	Content-Length: 1234
//	
//	<!doctype html>
//	<!-- Contenu HTML -->
