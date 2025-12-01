
#include "../../includes/requests/Post.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <fstream>
Post::Post(Request requ): Methods(requ), _body(requ._body), _contentLength(requ._ContentLength), _contentType(requ._ContentType)
{
    std::cout << GREEN << "Default Post constructor called" << RESET << std::endl;
}


static bool addContentToFile(std::string body, std::ofstream *newFile)
{
	newFile->write(body.c_str(), body.size());
	return (true);
}

const std::string Post::createResponse()
{
	std::string resp;
	std::ofstream newFile;
	Request dataError;

	newFile.open(this->_host + this->_location);
	if (!newFile.is_open())
		throw (ResponseError(400, "Heeeeu jsp", dataError));
	if (!addHost(&resp, this->_host))
		throw (ResponseError(400, "Heeeeu jsp", dataError));
	if (!addContentToFile(this->_body, &newFile)) // need some test
		throw (ResponseError(400, "Heeeeu jsp", dataError));
	if (!addLocation(&resp, this->_host, this->_location))
		throw (ResponseError(400, "Heeeeu jsp", dataError));
	if (addStartLine(&resp, this->_protocol, 201, "Created"))
		throw (ResponseError(400, "Heeeeu jsp", dataError));
	if (!addBody(&resp, newfile))
		throw (ResponseError(400, "Heeeeu jsp", dataError));

	return (resp);
}


// Example response:
//HTTP/1.1 201 Created
//Content-Type: application/json
//Location: http://example.com/users/123
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
