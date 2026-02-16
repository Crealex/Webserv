
#include "../../includes/requests/ResponseBuilder.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <sys/types.h>

ResponseBuilder::ResponseBuilder(const Request& dataError, const std::string& protocol): _requ(dataError), _protocol(protocol), _resp("")
{

}

ResponseBuilder& ResponseBuilder::contentType(const std::string& accept, const std::string& file)
{
	if (!addContentType(&this->_resp, accept, file))
		throw ResponseError(406, "Not acceptable", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::contentType(const std::string& type)
{
	if (!addContentType(&this->_resp, type))	
		throw ResponseError(500, "Can't add content type", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::date()
{
	if (!addDate(&this->_resp))
		throw ResponseError(500, "Can't add the date", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::lastModified(const std::string &path)
{
	if (!addLastModif(&this->_resp, path))
		throw ResponseError(500, "Can't add last modified", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::contentLength(ssize_t size)
{
	if (!addContentLenght(&this->_resp, size))
		throw ResponseError(500, "Can't add content lenght", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::contentLength(const std::string &path)
{
	if (!addContentLenght(&this->_resp, path))
		throw ResponseError(500, "Can't add content length", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::body(const std::string &content)
{
	if (!addBody(&this->_resp, content))
		throw ResponseError(500, "Can't add body", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::location(const std::string &loc)
{
	if (!addLocation(&this->_resp, loc))
		throw ResponseError(500, "Can't add location", this->_requ);
	return (*this);
}

ResponseBuilder& ResponseBuilder::startLine(unsigned int code, const std::string &mess)
{
	if (!addStartLine(&this->_resp, this->_protocol, code, mess))
		throw ResponseError(500, "Can't add first line", this->_requ);
	return (*this);
}

std::string ResponseBuilder::build()
{
	return (this->_resp);
}

ResponseBuilder& ResponseBuilder::append(const std::string &str)
{
	this->_resp.append(str);
	return (*this);
}
