
#include "../../includes/requests/Post.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <exception>
#include <fstream>
Post::Post(Request requ) : Methods(requ), _contentType(requ._ContentType), _contentLength(requ._ContentLength), _body(requ._body)
{
	std::cout << GREEN << "Default Post constructor called" << RESET << std::endl;
}

static bool addContentToFile(std::string body, std::ofstream *newFile)
{
	// newFile->write(body.c_str(), body.size());
	*newFile << body;
	return (true);
}

const std::string Post::createResponse(Config conf)
{
	// TODO: GERER L"AJOUT A UN FICHIER, pas juste le creer"
	std::string resp;
	std::ofstream newFile;
	Request dataError;
	std::string path;

	(void)_contentLength;
	// TODO: Path a revoir
	(void)conf;
	path = this->_host + this->_location;
	newFile.open(path.c_str(), std::ios::app);
	if (!newFile.is_open())
		throw(ResponseError(500, "Can't open new file or create it", dataError));
	if (!addContentType(&resp, this->_contentType))
		throw(ResponseError(500, "Can't add content type", dataError));
	if (!addContentToFile(this->_body, &newFile)) // need some test
		throw(ResponseError(500, "Can't add content file", dataError));
	if (!addLocation(&resp, this->_host, this->_location))
		throw(ResponseError(500, "Can't add Location", dataError));
	if (!addStartLine(&resp, this->_protocol, 201, "Created"))
		throw(ResponseError(500, "Can't add start line", dataError));
	if (!addBody(&resp, this->_body))
		throw(ResponseError(500, "Can't add body", dataError));
	newFile.close();
	return (resp);
}

// Example response:
// HTTP/1.1 201 Created
// Content-Type: application/json
// Location: http://example.com/users/123
//
//{
//  "message": "New user created",
//  "user": {
//    "id": 123,
//    "firstName": "Brian",
//    "lastName": "Smith",
//    "email": "brian.smith@example.com"
//  }
//}
//
//
// int main()
//{
//	Request requ;
//	requ._protocol = "HTTP/1.1";
//	requ._host = "../../www";
//	requ._location = "/testAlex/test.txt";
//	requ._ContentType = "text/txt";
//	requ._body = "Je suis un test et j'ai conscience de ma condition de simple test, je suis ok avec ça.";
//	Post test(requ);
//	try
//	{
//		test.createResponse();
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
