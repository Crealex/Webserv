#ifndef POST_HPP

# define POST_HPP

#include "methodsClass.hpp"
# include "../includes.hpp"

struct Request;

class Post: public Methods
{
    private:
		std::string	_contentType;
		std::string	_contentLength;
		std::string _body;
    public:
	Post(Request requ); // maybe mettre la struct de kilian en argument?
	const std::string	createResponse();
};

#endif // !POST_HPP
