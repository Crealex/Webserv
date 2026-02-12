#ifndef POST_HPP
# define POST_HPP

#include "Method.hpp"
# include "../includes.hpp"

class Post: public Methods
{
    private:
		std::string		_contentType;
		unsigned int	_contentLength;
		std::string		_body;
		void handlePostFile(std::string *resp, std::string boundary);
    public:
		Post(const Request &requ);
		const std::string	createResponse(const Server &srv);
};

#endif // !POST_HPP
