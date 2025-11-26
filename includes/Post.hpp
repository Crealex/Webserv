#ifndef POST_HPP

# define POST_HPP


# define POST_HPP
# include "includes.hpp"

struct request;

class Post
{
    private:
		
		std::string	_contentType;
		std::string	_contentLength;
		std::string _body;
    public:
	Post(request requ); // maybe mettre la struct de kilian en argument?
	const std::string	createResponse();
};

#endif // !POST_HPP
