
#include "../../includes/requests/Delete.hpp"
#include "../../includes/includes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include "../../includes/requests/ResponseBuilder.hpp"

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

static std::string createFileStr(std::ifstream &file)
{
	std::string fileStr;
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	fileStr.resize(size);
	file.read(&fileStr[0], size);

	return fileStr;
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
	std::string		target;
	Request			dataError;

	dataError = this->_createDataError();

	ResponseBuilder resp(dataError, this->_protocol);
	target = findTarget(this->_location, srv.getLocations(), dataError, "DELETE");
	path = srv.getRoot() + this->_location;
	if (access(path.c_str(), F_OK) == -1)
		return (noContent(this->_protocol, dataError));
	file.open(path.c_str());
	if (!file.is_open())
		throw (ResponseError(401, "Unauthorized", dataError));
	contentFile = createFileStr(file);


	if (std::remove(path.c_str()))
		throw (ResponseError(500, "can't remove content", dataError));

	return resp.date()
		.contentType("*/*", this->_location)
		.contentLength(contentFile.size())
		.body(contentFile)
		.startLine(200, "content deleted")
		.append("\r\n\r\n")
		.build();
}

