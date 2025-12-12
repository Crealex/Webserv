#include "../../includes/requests/ResponseError.hpp"
#include <exception>
#include "../../includes/requests/Request.hpp"
#include "../../includes/colors.hpp"

ResponseError::ResponseError(unsigned int code, std::string message, Request requ): std::exception(), Methods(requ), _code(code), _message(message)
{
	//std::cout << LIGHT_RED << "DEBUG: Created exception error response" << RESET << std::endl;
}

ResponseError::~ResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
}
const std::string ResponseError::createResponse()
{
	std::string resp;

	addStartLine(&resp, this->_protocol, this->_code, this->_message);
	return (resp);
}

