#ifndef RESPONSE_ERROR_HPP

#define RESPONSE_ERROR_HPP
#include "methodsClass.hpp"
#include <exception>

/**
 * @class ResponseError
 * @brief Called when an request is not correct or when a probleme come in the exectution of the request, the main constructor take the errorCode, a message et the dataStruct Request
 *
 */
class ResponseError : public std::exception, public Methods
{
  private:
	unsigned int _code;
	std::string _message;
	ResponseError();

  public:
	ResponseError(unsigned int code, std::string message, Request requ);
	const std::string createResponse() const throw();
	~ResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
};

#endif
