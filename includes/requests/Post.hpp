#ifndef POST_HPP
# define POST_HPP

#include "methodsClass.hpp"
# include "../includes.hpp"

class Post: public Methods
{
    private:
		std::string		_contentType;
		unsigned int	_contentLength;
		std::string		_body;
    public:
	Post(Request requ);
	const std::string	createResponse();
};

#endif // !POST_HPP
