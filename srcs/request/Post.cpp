
#include "../../includes/requests/Post.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
Post::Post(Request requ): Methods(requ), _body(requ._body), _contentLength(requ._ContentLength), _contentType(requ._ContentType)
{
    std::cout << GREEN << "Default Post constructor called" << RESET << std::endl;
}

const std::string Post::createResponse()
{
	std::string resp;

	return (resp);
}

