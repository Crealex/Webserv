#ifndef RESPONSE_ERROR_HPP

# define RESPONSE_ERROR_HPP
# include "methodsClass.hpp"
# include <exception>

class ResponseError: public std::exception, public Methods
{
	private:
		unsigned int	_code;
		std::string		_message;
		ResponseError();
	public:
		ResponseError(unsigned int code, std::string message, Request requ);
		const std::string createResponse();
};


#endif
