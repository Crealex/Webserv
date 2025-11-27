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
#endif

