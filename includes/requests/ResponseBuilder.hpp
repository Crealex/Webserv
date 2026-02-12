#ifndef RESPONSE_BUILDER

# define RESPONSE_BUILDER
# include "Method.hpp"

class ResponseBuilder {

private:
	std::string _resp;
	const Request &_requ;
	std::string _protocol;

public:
	ResponseBuilder(const Request& dataError, const std::string& protocol);
    ResponseBuilder& contentType(const std::string& accept, const std::string& file);
    ResponseBuilder& contentType(const std::string& type);
    ResponseBuilder& date();
    ResponseBuilder& lastModified(const std::string& path);
    ResponseBuilder& contentLength(const std::string& path);
    ResponseBuilder& contentLength(ssize_t size);
    ResponseBuilder& body(const std::string& content);
    ResponseBuilder& location(const std::string& loc);
    ResponseBuilder& startLine(unsigned int code, const std::string& message);
    std::string build();
};



#endif // RESPONSE_BUILDER
