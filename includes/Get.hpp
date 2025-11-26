#ifndef GET_HPP

# define GET_HPP
# include "includes.hpp"

struct request;

class Get
{
    private:
		std::string	_userAgent;
		std::string	_accept;

    public:
        Get(request requ);
		const std::string	createResponse(); // meme reflexion que pour post
};
#endif // !GET_HPP
