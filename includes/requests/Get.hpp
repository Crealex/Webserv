#ifndef GET_HPP

# define GET_HPP
# include "../includes.hpp"
#include "methodsClass.hpp"

struct Request;

class Get: public Methods
{
    private:
		std::string	_userAgent;
		std::string	_accept;

    public:
        Get(Request requ);
		const std::string	createResponse(); // meme reflexion que pour post
};
#endif // !GET_HPP
