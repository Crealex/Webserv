
#include "../../includes/requests/Post.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <cstddef>
#include <exception>
#include <fstream>
#include <string>
#include <sys/types.h>
Post::Post(Request requ) : Methods(requ), _contentType(requ._ContentType), _contentLength(requ._ContentLength), _body(requ._body)
{
	std::cout << GREEN << "Default Post constructor called" << RESET << std::endl;
}

std::string extractBoundary(std::string contentType)
{
	std::string res;
	unsigned int find;

	find = contentType.find("boundary");
	if (find < contentType.length())
	{
		res = contentType.erase(0, contentType.find('=') + 1);
		std::cout << "res boundary: " << res << std::endl;
	}
	while (!res.empty() && (res[res.length() - 1] == '\r' || res[res.length() - 1] == '\n'))                                        
		res.erase(res.length() - 1); 
	return (res);
}

static bool addContentToFile(std::string body, std::ofstream *newFile)
{
	*newFile << body;
	return (true);
}

	//file.seekg(0, std::ios::end);
	//size_t size = file.tellg();
	//file.seekg(0, std::ios::beg);
	//char* buffer = new char[size];
	//file.read(buffer, size);
	//std::string fileStr(buffer, size);
	//delete[] buffer;
// TODO: A continuer le moment venu...
void Post::handlePostFile(std::string *resp, std::string boundary)
{
	std::size_t start;
	std::size_t end;
	std::size_t length;

	(void)resp;
	//if (this->_body.find("content-type"))
	//	this->_contentType = this->_body.erase(0, 12);
	start = this->_body.find("\r\n\r\n") + 4;
	end = this->_body.rfind("--" + boundary);
	std::cout << "Boundary recherché: [" << boundary << "]" << std::endl;  
	std::cout << "Recherche de: [\\r\\n--" << boundary << "]" << std::endl;
																			 
	length = end - start;
	this->_body = this->_body.substr(start, length);
}

const std::string Post::createResponse(Server srv)
{
	std::string resp;
	std::ofstream newFile;
	Request dataError;
	std::string path;
	std::string boundary;
	std::string target;
	ssize_t bodySize;

	dataError._protocol = this->_protocol;
	dataError._host = this->_host;
	dataError._location = this->_location;
	target = findTarget(this->_location, srv.getLocations(), dataError, "POST");
	path = srv.getRoot() + target;
	if (this->_contentType.find("multipart/form-data") < this->_contentType.size())
	{
		boundary = extractBoundary(this->_contentType);
		handlePostFile(&resp, boundary);
		newFile.open(path.c_str(), std::ios::binary);
	}
	else {
		newFile.open(path.c_str(), std::ios::app);
	}
	bodySize = this->_body.size();
	//std::cout << "Body in post: " << this->_body << std::endl;

	if (!newFile.is_open())
		throw (ResponseError(401, "Unauthorized", dataError));
	if (!addContentType(&resp, this->_contentType))
		throw(ResponseError(500, "Can't add content type", dataError));
	if (!addContentToFile(this->_body, &newFile)) // need some test
		throw(ResponseError(500, "Can't add content file", dataError));
	if (!addLocation(&resp, this->_host, this->_location))
		throw(ResponseError(500, "Can't add Location", dataError));
	resp.append("\n");
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, bodySize))
		throw ResponseError(500, "can't add content length", dataError);
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
