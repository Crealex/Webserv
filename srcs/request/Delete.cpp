
#include "../../includes/requests/Delete.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/Request.hpp"
#include <fstream>
#include <unistd.h>

Delete::Delete(Request requ) : Methods(requ)
{
	std::cout << GREEN << "Default Delete constructor called" << RESET << std::endl;
}

/**
 * @brief
 *
 * @param protocol
 */
static std::string noContent(std::string protocol)
{
	std::string resp;

	resp.append(protocol + "204" + "No content");
	if (!addDate(&resp))
		throw (ResponseError());
}

const std::string Delete::createResponse()
{
	std::ifstream	file;
	std::string		path;

	path = this->_host + this->_location;
	if (access(path.c_str(), F_OK) == -1)
	{
		return (noContent(this->_protocol)); // créer la version si nos content
	}


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
