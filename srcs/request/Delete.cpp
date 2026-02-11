
#include "../../includes/requests/Delete.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <fstream>
#include <string>

Delete::Delete(const Request &requ) : Methods(requ)
{
}

/**
 * @brief handling when there is no content
 *
 * @param protocol
 */
static std::string noContent(std::string protocol, Request dataError)
{
	std::string resp;

	resp.append(protocol + " 204 " + "No content" + "\n");
	if (!addDate(&resp))
		throw (ResponseError(500, "can't add start line\n", dataError));
	resp.append("Server: webserv\n\n");
	return (resp);
}

/**
 * @brief Build the http response when a delete request is recieve
 *
 * @param srv The class Server
 * @return A string with the response to send
 */
const std::string Delete::createResponse(const Server &srv)
{
	std::ifstream	file;
	std::string		contentFile;
	std::string		path;
	std::string		resp;
	std::string		target;
	Request			dataError;

	dataError = this->_createDataError();

	target = findTarget(this->_location, srv.getLocations(), dataError, "DELETE");
	// TODO: Verif si target est un file ou un dossier ou une redirection;
	path = srv.getRoot() + this->_location; // INFO: Peut etre un file ou un dossier
	if (access(path.c_str(), F_OK) == -1)
		return (noContent(this->_protocol, dataError));
	if (!addDate(&resp))
		throw (ResponseError(500, "can't add date", dataError));
	if (!addContentType(&resp, "*/*", this->_location))
		throw (ResponseError(500, "can't add content type", dataError));
	file.open(path.c_str());
	if (!file.is_open())
		throw (ResponseError(401, "Unauthorized", dataError));
	std::getline(file, contentFile, '\0');
	if (!addContentLenght(&resp, path))
		throw (ResponseError(500, "can't add content length", dataError));
	if (!addBody(&resp, contentFile))
		throw (ResponseError(500, "can't add body", dataError));
	if (std::remove(path.c_str()))
		throw (ResponseError(500, "can't remove content", dataError));
	if (!addStartLine(&resp, this->_protocol, 200, "content deleted"))
		throw (ResponseError(500, "can't remove content", dataError));

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

//int main()
//{
//	Request requ;
//	requ._protocol = "HTTP/1.1";
//	requ._host = "../../www";
//	requ._location = "/testAlex/test.txt";
//	requ._ContentType = "text/txt";
//	requ._body = "Je suis un test et j'ai conscience de ma condition de simple test, je suis ok avec ça.";
//	Delete test(requ);
//	try
//	{
//		std::cout << test.createResponse() << std::endl;
//	} catch (ResponseError &e)
//	{
//		std::cerr << RED << e.createResponse() << std::endl;
//	} catch (std::exception &e)
//	{
//		std::cout << e.what() << std::endl;
//		return (1);
//	}
//	return 0;
//}
