
#include "../../includes/requests/Post.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
Post::Post(const Request &requ) : Methods(requ), _contentType(requ.getContentType()), _contentLength(requ.getContentLength()), _body(requ.getBody())
{
}

std::string extractBoundary(std::string contentType)
{
	std::string res;
	unsigned int find;

	find = contentType.find("boundary");
	if (find < contentType.length())
	{
		res = contentType.erase(0, contentType.find('=') + 1);
		//std::cout << "res boundary: " << res << std::endl;
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

void Post::handlePostFile(std::string *resp, std::string boundary)
{
	std::size_t start;
	std::size_t end;
	std::size_t length;

	(void)resp;
	start = this->_body.find("\r\n\r\n") + 4;
	end = this->_body.rfind("--" + boundary);
																			 
	length = end - start;
	this->_body = this->_body.substr(start, length);
}

/**
 * @brief Build the http response when a request post is recieve
 *
 * @param srv The class Server
 * @return A string with the resopnse to send
 */
const std::string Post::createResponse(const Server &srv)
{
	std::string resp;
	std::ofstream newFile;
	Request dataError;
	std::string path;
	std::string boundary;
	std::string target;
	ssize_t bodySize;

	dataError = this->_createDataError();
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

	if (!newFile.is_open())
		throw (ResponseError(401, "Unauthorized", dataError));
	if (!addContentType(&resp, this->_contentType))
		throw(ResponseError(500, "Can't add content type", dataError));
	if (!addContentToFile(this->_body, &newFile)) // need some test
		throw(ResponseError(500, "Can't add content file", dataError));
	if (!addLocation(&resp, path))
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

