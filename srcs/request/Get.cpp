#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include <cstdio>
#include <ctime>
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

static std::string findMimeType(struct stat fileData)
{
	std::cout << fileData.st_mode << std::endl;
	return ("test/html");
}

static bool addStartLine(std::string *resp, std::string protocol, unsigned int code)
{
	return (true);
}

//	Content-Type: text/html; charset=UTF-8
static bool addContentType(std::string *resp, std::string accept, struct stat fileData)
{
	std::string contentType;

	// detecter le type de contenu demandé
	contentType = findMimeType(fileData);
	if (contentType != accept && accept != "*/*")
		return (false);
	return (true);
}

static bool addDate(std::string *resp)
{
	std::tm time;
	return (true);
}

static bool addLastModif(std::string *resp, std::string pathTarget)
{
	struct stat buff;

	stat(pathTarget.c_str(), &buff);
	return (true);
}

static bool addContentLenght(std::string *resp, std::string file)
{
	unsigned int size;
	size = file.size();
	return (true);
}

static bool addBody(std::string *resp, std::string file)
{
	return (true);
}

const std::string Get::createResponse()
{
	std::string		resp;
	std::ifstream	target;
	std::string		file;
	struct stat		fileData;
	
	target.open(this->_host + this->_location);
	if (!target.is_open())
		return (sendError(666));
	stat((this->_host + this->_location).c_str(), &fileData);
	std::getline(target, file, '\0');
	if (!addContentType(&resp, this->_accept, fileData))
		return (sendError(666));
	if (!addDate(&resp))
		return (sendError(666));
	if (!addContentLenght(&resp, file))
		return (sendError(666));
	if (!addBody(&resp, file))
		return (sendError(666));
	if (!addStartLine(&resp, this->_protocol, 200))
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
