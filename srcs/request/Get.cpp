#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/requests/ResponseError.hpp"

Get::Get(const Request &requ): Methods(requ), _userAgent(requ.getUserAgent()), _accept(requ.getAccept()) 
{
    // std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}


// *** Create response
//
static std::string createFileStr(std::ifstream &file)
{
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	char* buffer = new char[size];
	file.read(buffer, size);
	std::string fileStr(buffer, size);
	delete[] buffer;

	return fileStr;
}

/**
 * @brief Identify the status and the message associted
 *
 * @param srv The class Server
 * @return A pair with the a unisgned int (the code) and a std::string (the message)
 */
std::pair<unsigned int, std::string> Get::_findCodeMess(const Server &srv)
{
	std::pair<unsigned int, std::string> ret;
	int finded = -1;

	for (size_t i = 0; i < srv.getLocations().size(); i++)
	{
		if (this->_location == srv.getLocations().at(i).getPath())
		{
			finded = i;
			break ;
		}
	}
	if (finded == -1)
		return (std::pair<unsigned int, std::string>(200, "OK"));
	if (srv.getLocations().at(finded).getReturn().first.empty())
		return (std::pair<unsigned int, std::string>(200, "OK"));
	ret.first = srv.getLocations().at(finded).getReturn().second;
	switch (ret.first) 
	{
		case 301:	ret.second = "Moved Permanently";
					break;
		case 307:	ret.second = "Temporary Redirect";
					break;
		case 308:	ret.second = "Permanent Redirect";
					break;
		default:	ret.second = "undefinded";
					break;
	}
	return (ret);
}

/**
 * @brief building the response for get request
 *
 * @param srv The server class
 * @return A string with de response to send
 */
const std::string Get::createResponse(const Server &srv)
{
	std::string		resp;
	std::ifstream	file;
	std::string		path;
	Request			dataError;
	std::string		target;
	std::string fileStr;
	std::pair<unsigned int, std::string> codeMess;
	bool isRedir = 0;
	
	dataError = this->_createDataError();
	dataError.setAccept(this->_accept);
	dataError.setUserAgent(this->_userAgent);

	target = findTarget(this->_location, srv.getLocations(), dataError, "GET");
	if (target.find("http") == std::string::npos)
	{
		path = srv.getRoot() + "/" + target; // TODO: Peut-etre retirer le /
		file.open(path.c_str());
		// *************************
		if (!file.is_open())
			throw ResponseError(404, "Not found", dataError);
		fileStr = createFileStr(file);
	}
	else
	{
		isRedir = 1;
		path = target;
	}
	//std::cout << "complete path to get: " << path << std::endl;

	codeMess = _findCodeMess(srv);
	if (isRedir)
	{
		addLocation(&resp, path);
		addContentType(&resp, this->_accept, path);
		addDate(&resp);
		addContentLenght(&resp, 0); // Content-Length = 0
		addStartLine(&resp, this->_protocol, codeMess.first, codeMess.second);
		resp.append("\n\r\n\r");
		return (resp);
	}
	if (!addContentType(&resp, this->_accept, path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path))
		throw ResponseError(500, "can't add content length", dataError);
	if (!addBody(&resp, fileStr))
		throw ResponseError(500, "can't add body", dataError);
	if (!addStartLine(&resp, this->_protocol, codeMess.first, codeMess.second))
		throw ResponseError(500, "can't add start line", dataError);

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
