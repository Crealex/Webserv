
#include "../../includes/includes.hpp"
#include "../../includes/requests/MimeTypes.hpp"
#include <ctime>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>


// INFO: All prototypes are in methodClass.hpp

//	HTTP/1.1 200 OK
bool addStartLine(std::string *resp, std::string protocol, unsigned int code, std::string mess)
{
	std::stringstream ss;

	ss << code;
	try {
		resp->insert(0, protocol + " " + ss.str() + " " + mess + "\n");
	} catch (std::exception &e) {
		return (false);
	}

	return (true);
}

std::string findMimeType(std::string file)
{
	std::string extension;

	std::cout << "file in " << file << std::endl;
	extension = file.substr(file.find_last_of(".") + 1, file.length());
	return MimeTypes::getType(extension);
}
//	Content-Type: text/html; charset=UTF-8
bool addContentType(std::string *resp, std::string accept, std::string file)
{
	std::string contentType;
	std::stringstream acceptSs(accept);
	std::string type;

	contentType = findMimeType(file);
	std::cout << "accept in addContentType" << accept << std::endl;
	while (std::getline(acceptSs, type,  ','))
	{
		std::cout << "type: " << type << ", Content-Type: " << contentType << std::endl;
		if (contentType == type || type == "*/*")
		{
			resp->append("Content-Type: " + contentType + "\n");
			return (true);
		}
	}
	return (false);
}

// For post.cpp
bool addContentType(std::string *resp, std::string type)
{
	try {
		resp->append("Content-Type: " + type + "\n");	
	} catch (std::exception &e) {
		return (false);
	}
	return (true);
}

//	Date: Fri, 21 Jun 2024 14:18:33 GMT
bool addDate(std::string *resp)
{
	char buff[100];
	tm *time;
	time_t tt;

	std::time(&tt);
	time = std::localtime(&tt);
	if (!time)
		return (false);
	std::strftime(buff, 100, "Date: %a, %d %b %Y %X GMT\n", time);
	resp->append(buff);
	return (true);
}

//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
bool addLastModif(std::string *resp, std::string pathTarget)
{
	struct stat buff;
	char date[100];
	tm *time;
	time_t tt;
	std::string dateString;

	if (stat(pathTarget.c_str(), &buff))
		return (false);
	tt = buff.st_mtim.tv_sec;
	try {
		time = std::localtime(&tt);
		std::strftime(date, 100, "%a, %d %b %Y %X GMT\n", time);
		dateString = date;
		resp->append("Last-Modified: " + dateString);
	}
	catch (std::exception &e){
		return (false);
}
	return (true);
}

//	Content-Length: 1234
bool addContentLenght(std::string *resp, std::string file)
{
	unsigned int size;
	std::stringstream ss;

	size = file.size();
	ss << size;
	try {
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e) {
		return (false);
	}
	return (true);
}

//	<!doctype html>
//	<!-- Contenu HTML -->
bool addBody(std::string *resp, std::string file)
{
	resp->append("\n" + file);
	return (true);
}

bool addLocation(std::string *resp, std::string host, std::string location)
{
	try
	{
		resp->append("Location: " + host + location);
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

