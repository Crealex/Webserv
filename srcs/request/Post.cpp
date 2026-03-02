
#include "../../includes/requests/Post.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseBuilder.hpp"
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
	}
	while (!res.empty() && (res[res.length() - 1] == '\r' || res[res.length() - 1] == '\n'))
		res.erase(res.length() - 1);
	return (res);
}

void Post::_handlePostFile(std::string boundary)
{
	std::size_t start;
	std::size_t end;
	std::size_t length;

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
	std::ofstream newFile;
	Request dataError;
	std::string path;
	std::string boundary;
	std::string target;
	ssize_t bodySize;

	dataError = this->_createDataError();
	ResponseBuilder resp(dataError, this->_protocol);
	target = findTarget(this->_location, srv.getLocations(), dataError, "POST");
	path = srv.getRoot() + target;
	if (this->_contentType.find("multipart/form-data") < this->_contentType.size())
	{
		boundary = extractBoundary(this->_contentType);
		_handlePostFile(boundary);
		newFile.open(path.c_str(), std::ios::binary);
	} else
	{
		newFile.open(path.c_str(), std::ios::app);
	}
	bodySize = this->_body.size();

	if (!newFile.is_open())
		throw(ResponseError(401, "Unauthorized", dataError));

	newFile << this->_body;
	return resp.contentType(this->_contentType)
		.location(path)
		.lastModified(path)
		.contentLength(bodySize)
		.body(this->_body)
		.startLine(201, "Created")
		.build();
}
