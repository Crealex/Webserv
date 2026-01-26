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


// *** Create response

const std::string Get::createResponse(Server srv)
{
	std::string		resp;
	std::ifstream	file;
	std::string		path;
	Request			dataError;
	std::string		target;
	
	dataError._protocol = this->_protocol;
	dataError._host = this->_host;
	dataError._accept = this->_accept;
	dataError._location = this->_location;
	dataError._userAgent = this->_userAgent;

	target = findTarget(this->_location, srv.getLocations(), dataError, "GET");
	// TODO: Verif si target est un file ou un dossier ou une redirection;
	path = srv.getRoot() + "/" + target; // TODO: Peut-etre retirer le /
	std::cout << "complete path to get: " << path << std::endl;
	file.open(path.c_str());
	// *************************
	if (!file.is_open())
		throw ResponseError(404, "Not found", dataError);

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	char* buffer = new char[size];
	file.read(buffer, size);
	std::string fileStr(buffer, size);
	delete[] buffer;

	if (!addContentType(&resp, this->_accept, path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path))
		throw ResponseError(500, "can't add content lenght", dataError);
	if (!addBody(&resp, fileStr))
		throw ResponseError(500, "can't add body", dataError);
	if (!addStartLine(&resp, this->_protocol, 200, "OK"))
		throw ResponseError(500, "can't add start line", dataError);

	std::cout << "end of get" << std::endl;
	return (resp);
}


// *** TEST MAIN ***

// static bool	isDuplicateServer(Server temp, std::vector<Server> res)
// {
// 	int	sizeRes;
//
// 	sizeRes = res.size();
// 	for (int i = 0; i < sizeRes; i++)
// 	{
// 		if (res[i].getHostname() == temp.getHostname())
// 			return (true);
// 	}
// 	return (false);
// }
//
// static std::vector<Server>	createServers(std::string path)
// {
// 	std::vector<Server>	res;
// 	int					sizeStructSrv;
// 	std::vector<server>	structServers;
//
// 	structServers = createVectStructSrv(path);
// 	sizeStructSrv = structServers.size();
// 	for (int i = 0; i < sizeStructSrv; i++)
// 	{
// 		Server	temp(structServers[i]);
// 		if (isDuplicateServer(temp, res))
// 			throw std::invalid_argument(RED "Error : this server exists already" RESET);
// 		res.push_back(temp);
// 	}
// 	return (res);
// }
//
// int main(void)
// {
// 	Request requ;
//
// 	requ._accept = "text/html";
// 	requ._host = "pipou";
// 	requ._location = "/";
// 	requ._protocol = "HTTP/1.1";
// 	requ._userAgent = "Firefox";
//
// 	std::vector<Server> srvs;
// 	try 
// 	{
// 		srvs = createServers("danalexian.conf");
// 		Get		resp(requ);
// 		std::cout << "resp: " << std::endl;
// 		std::cout << resp.createResponse(srvs.at(0)) << std::endl;
//
// 	}
// 	catch (ResponseError &e)
// 	{
// 		std::cout << e.createResponse(srvs.at(0)) << std::endl;
// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cout << e.what() << std::endl;
// 	}
//
// }

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
