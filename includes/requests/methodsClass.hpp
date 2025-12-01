#ifndef METHODS_CLASS_HPP

# define METHODS_CLASS_HPP
# include "../includes.hpp"

struct Request;

class Methods {
	protected:
		std::string		_method;
		std::string		_location;
		std::string		_protocol;
		std::string		_host;
		Methods(Request requ);
	public:
		virtual const std::string createResponse() = 0;

	
};

// addLineToResp.cpp

bool	addStartLine(std::string *resp, std::string protocol, unsigned int code);
bool	addContentType(std::string *resp, std::string accept, std::string file);
bool	addDate(std::string *resp);
bool	addLastModif(std::string *resp, std::string pathTarget);
bool	addContentLenght(std::string *resp, std::string file);
bool	addBody(std::string *resp, std::string file);

#endif

