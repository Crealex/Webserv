
#include "../../includes/requests/ResponseBuilder.hpp"
#include "../../includes/requests/ResponseError.hpp"

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
