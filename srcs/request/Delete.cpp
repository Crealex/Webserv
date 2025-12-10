
#include "../../includes/requests/Delete.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <fstream>
#include <iterator>
#include <pstl/glue_algorithm_defs.h>
#include <string>

Delete::Delete(Request requ) : Methods(requ)
{
	std::cout << GREEN << "Default Delete constructor called" << RESET << std::endl;
}

/**
 * @brief
 *
 * @param protocol
 */
static std::string noContent(std::string protocol, Request dataRequ)
{
	std::string resp;

	resp.append(protocol + "204" + "No content");
	if (!addDate(&resp))
		throw (ResponseError(500, "can't add start line", dataRequ));
	resp.append("Server: webserv");
	return (resp);
}

const std::string Delete::createResponse()
{
	std::ifstream	file;
	std::string		contentFile;
	std::string		path;
	std::string		resp;
	Request			dataRequ;

	path = this->_host + this->_location;
	if (access(path.c_str(), F_OK) == -1)
		return (noContent(this->_protocol, dataRequ));
	if (std::remove(path.c_str()))
		throw (ResponseError(500, "can't remove content", dataRequ));
	if (!addDate(&resp))
		throw (ResponseError(500, "can't add date", dataRequ));
	if (!addContentType(&resp, "*/*", this->_location))
		throw (ResponseError(500, "can't add content type", dataRequ));
	file.open(path);
	if (!file.is_open())
		throw (ResponseError(ResponseError(500, "can't open the file", dataRequ)));
	std::getline(file, contentFile, '\0');
	if (!addContentLenght(&resp, contentFile))

	


	return (resp);
}

// If content exist:
//HTTP/1.1 200 OK
//Content-Type: text/html; charset=UTF-8
//Date: Fri, 21 Jun 2024 14:18:33 GMT
//Content-Length: 1234
//
//<html lang="en-US">
//  <body>
//    <h1>File "file.html" deleted.</h1>
//  </body>
//</html>

//if content doesn't exist:
//HTTP/1.1 204 No Content
//Date: Wed, 04 Sep 2024 10:16:04 GMT
