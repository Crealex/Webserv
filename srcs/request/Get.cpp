#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <exception>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include "../../includes/Server.hpp"
#include "../../includes/requests/ResponseError.hpp"

Get::Get(Request requ): Methods(requ), _userAgent(requ._userAgent), _accept(requ._accept) 
{
    std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}

std::string findTarget(std::string locPath, std::vector<Location> loc, Request dataError)
{
	unsigned int i = 0;

	while (loc.size() > i)
	{
		if (loc.at(i).getPath() == locPath)
		{
			if (loc.at(i).getAutoIndex())
				return loc.at(i).getPath();
			else if (!loc.at(i).getIndex().empty())
				return (loc.at(i).getIndex());
			else if (!loc.at(i).getReturn().first.empty())
				return (loc.at(i).getReturn().first);
			else
				throw ResponseError(404, "Not found", dataError);
		}
		i++;
	}
	throw ResponseError(404, "Not found", dataError);
	return std::string();
}

// *** Create response

const std::string Get::createResponse(Server srv)
{
	std::string		resp;
	std::ifstream	target;
	std::string		path;
	Request			dataError;
	
	dataError._protocol = this->_protocol;
	dataError._host = this->_host;
	dataError._accept = this->_accept;
	dataError._location = this->_location;
	dataError._userAgent = this->_userAgent;
	// root + sitename + (si pas fichier precis) defaultfile
	path = srv.getRoot() + findTarget(this->_location, srv.getLocations(), dataError);
	target.open(path.c_str());
	// *************************
	if (!target.is_open())
		throw ResponseError(404, "Not found", dataError);

	target.seekg(0, std::ios::end);
	size_t size = target.tellg();
	target.seekg(0, std::ios::beg);
	char* buffer = new char[size];
	target.read(buffer, size);
	std::string file(buffer, size);
	delete[] buffer;

	if (!addContentType(&resp, this->_accept, path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path))
		throw ResponseError(500, "can't add content lenght", dataError);
	if (!addBody(&resp, file))
		throw ResponseError(500, "can't add body", dataError);
	if (!addStartLine(&resp, this->_protocol, 200, "OK"))
		throw ResponseError(500, "can't add start line", dataError);

	return (resp);
}


// *** TEST MAIN ***

int main(void)
{
	Request requ;

	requ._accept = "text/html";
	requ._host = "../../www";
	requ._location = "/demo/index.html";
	requ._protocol = "HTTe/1.1";
	requ._userAgent = "Firefox";
	Server srv("../../danalexian.conf");

	try 
	{
		Get		resp(requ);
		std::cout << "resp: " << std::endl;
		std::cout << resp.createResponse(srv) << std::endl;

	}
	catch (ResponseError &e)
	{
		std::cout << e.createResponse(srv) << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

}

// compile: c++ -Werror -Wall -Werror request/MethodsClass.cpp parsing_config/createStructV2.cpp parsing_config/Location.cpp Server.cpp request/Get.cpp request/ResponseError.cpp
//*** RESPONSE EXAMPLE ***
//	HTTP/1.1 200 OK
//	Content-Type: text/html; charset=UTF-8
//	Date: Fri, 21 Jun 2024 14:18:33 GMT
//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
//	Content-Length: 1234
//	
//	<!doctype html>
//	<!-- Contenu HTML -->
