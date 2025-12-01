
 
#include "../../includes/includes.hpp"
#include <sstream>
#include "../../includes/requests/MimeTypes.hpp"
#include <ctime>
#include <sys/stat.h>


// TODO: Checker les retours des toutes les fonctions appelée pour bien gerer les erreurs

//	HTTP/1.1 200 OK
bool addStartLine(std::string *resp, std::string protocol, unsigned int code, std::string mess)
{
	std::stringstream ss;

	ss << code;
	resp->insert(0, protocol + " " + ss.str() + " " + mess + "\n"); // TODO: Voir comment mettre des messages personnalisé
	

	return (true);
}

std::string findMimeType(std::string file)
{
	std::string extension;
	
	extension = file.substr(file.find_last_of(".") + 1, file.length());
	return MimeTypes::getType(extension);
}
//	Content-Type: text/html; charset=UTF-8
bool addContentType(std::string *resp, std::string accept, std::string file)
{
	std::string contentType;

	contentType = findMimeType(file);
	if (contentType != accept && accept != "*/*")
		return (false);

	resp->append("Content-Type: " + contentType + "\n");
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
	std::strftime(buff, 100, "Date: %a, %d %b %Y %X GMT\n", time);
	resp->append(buff);
	return (true);
}

//	Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
bool addLastModif(std::string *resp, std::string pathTarget)
{
	struct stat	buff;
	char date[100];
	tm *time;
	time_t tt;
	std::string dateString;

	 if (stat(pathTarget.c_str(), &buff))
		return (false);
	tt = buff.st_mtim.tv_sec;
	time = std::localtime(&tt);
	std::strftime(date, 100, "%a, %d %b %Y %X GMT\n", time);
	dateString = date;
	resp->append("Last-Modified: " + dateString);
	return (true);
}

//	Content-Length: 1234
bool addContentLenght(std::string *resp, std::string file)
{
	unsigned int		size;
	std::stringstream	ss;

	size = file.size();
	ss << size;
	resp->append("Content-Lenght: " + ss.str() + "\n");
	return (true);
}


//	<!doctype html>
//	<!-- Contenu HTML -->
bool addBody(std::string *resp, std::string file)
{
	resp->append("\n" + file);
	return (true);
}
