
#include "../../includes/includes.hpp"
#include "../../includes/requests/MimeTypes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"


// INFO: All prototypes are in methodClass.hpp

// UTILS

std::string findTarget(std::string locPath, std::vector<Location> loc, Request dataError, std::string method)
{
	unsigned int i = 0;

	while (loc.size() > i)
	{
		if (loc.at(i).getPath() == locPath)
		{
			if (!loc.at(i).getMethodValue(method))
			{
				if (loc.at(i).getReturn().first.empty())
					throw ResponseError(405, "Method not allowed", dataError);
				return (loc.at(i).getReturn().first);
			}
			if (loc.at(i).getAutoIndex())
				return loc.at(i).getPath();
			else if (!loc.at(i).getIndex().empty())
				return (loc.at(i).getIndex());
			else if (!loc.at(i).getReturn().first.empty())
				return (loc.at(i).getReturn().first);
		}
		i++;
	}
	return (locPath);
}


// *** ADDING LINE TO RESPONSE

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

	//std::cout << "file in " << file << std::endl;
	extension = file.substr(file.find_last_of(".") + 1, file.length());
	return MimeTypes::getType(extension);
}
//	Content-Type: text/html; charset=UTF-8
bool addContentType(std::string *resp, std::string accept, std::string file)
{
	std::string contentType;
	std::stringstream acceptSs(accept);
	std::string type;

	if (file.find("http") < file.size())
	{
		resp->append("Content-Type: text/html\n");
		return (true);
	}
	contentType = findMimeType(file);
	while (std::getline(acceptSs, type,  ','))
	{
		//std::cout << "type: " << type << ", Content-Type: " << contentType << std::endl;
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
	std::ifstream file(pathTarget.c_str());
	std::string dateString;

	if (!file.is_open())
	{
		std::time(&tt);
		time = std::localtime(&tt);
		std::strftime(date, 100, "Last-Modified: %a, %d %b %Y %X GMT\n", time);
		resp->append(date);

		return (true);
	}
	stat(pathTarget.c_str(), &buff);
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

bool addContentLenght(std::string *resp, std::string path)
{
	unsigned int size;
	std::stringstream ss;
	struct stat buff;

	if (path.find("http") < path.size())
	{
		resp->append("Content-Length: 0\n");
		return true;
	}
	stat(path.c_str(), &buff);
	size = buff.st_size;
	ss << size;
	try {
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e) {
		return (false);
	}
	return (true);
}

//	Content-Length: 1234
bool addContentLenght(std::string *resp, std::string path, std::string fileStr)
{
	unsigned int size;
	std::stringstream ss;
	struct stat buff;
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		size = fileStr.size();
		ss << size;
		resp->append("Content-Length: " + ss.str() + "\n");
		return (true);
	}
	stat(path.c_str(), &buff);
	size = buff.st_size;
	ss << size;
	try {
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e) {
		return (false);
	}
	return (true);
}

bool addContentLenght(std::string *resp, ssize_t bodySize)
{
	std::stringstream ss;

	ss << bodySize;
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
	resp->append("\n" + file + "\r\n\r\n");
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

bool addLocation(std::string *resp, std::string location)
{
	try
	{
		resp->append("Location: " + location + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}
