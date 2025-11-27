#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/MimeTypes.hpp"
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

static std::string findMimeType(std::string file)
{
	std::string extension;
	
	extension = file.substr(file.find_last_of(".") + 1, file.length());
	return MimeTypes::getType(extension);
}

//	HTTP/1.1 200 OK
static bool addStartLine(std::string *resp, std::string protocol, unsigned int code)
{
	return (true);
}

//	Content-Type: text/html; charset=UTF-8
static bool addContentType(std::string *resp, std::string accept, std::string file)
{
	std::string contentType;

	contentType = findMimeType(file);
	if (contentType != accept && accept != "*/*")
		return (false);

	resp->append("Content-Type: " + contentType + "\n");
	return (true);
}

//	Date: Fri, 21 Jun 2024 14:18:33 GMT
static bool addDate(std::string *resp)
{
	std::tm time;
	std::string date;

	mktime(&time);
	// TODO: Mater les fonctions pour formater la date sans devoir le faire a la main
	switch (time.tm_wday)
	{
		case 0:
			date.append("Mon");
		case 1:
			date.append("Tue");
		case 2:
			date.append("Wen");
	}
	time.tm_mday; // day of month
	time.tm_mon; // month
	time.tm_year; // year - 1900
	time.tm_hour; // hour 
	time.tm_min; // minutes
	time.tm_sec; // seconds
	time.tm_zone; // timezone (pas sur, sinon rajouter a la main GMT)
	return (true);
}

//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
static bool addLastModif(std::string *resp, std::string pathTarget)
{
	struct stat buff;

	stat(pathTarget.c_str(), &buff);
	return (true);
}

//	Content-Length: 1234
static bool addContentLenght(std::string *resp, std::string file)
{
	unsigned int size;
	size = file.size();
	return (true);
}

//	<!doctype html>
//	<!-- Contenu HTML -->
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
	if (!addContentType(&resp, this->_accept, this->_location)) // maked, need to test
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


// *** TEST MAIN ***

//int main(void)
//{
//	std::string file = "trdy/sdcs/dsc.css";
//	std::cout << "test with " << file << ": " << findMimeType(file) << std::endl;
//	file = "caca.boudin.js";
//	std::cout << "test with " << file << ": " << findMimeType(file) << std::endl;
//	file = "pipou.avif";
//	std::cout << "test with " << file << ": " << findMimeType(file) << std::endl;
//	file = "caca/bite/trou/n.mp4";
//	std::cout << "test with " << file << ": " << findMimeType(file) << std::endl;
//	file = "je/sais/pas.caca";
//	std::cout << "test with " << file << ": " << findMimeType(file) << std::endl;
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
