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
		void handlePostFile(std::string *resp, std::string boundary);
    public:
		Post(Request requ);
		const std::string	createResponse(Config conf);
};

#endif // !POST_HPP
