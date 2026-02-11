#ifndef GET_HPP
# define GET_HPP

# include "../includes.hpp"
# include "methodsClass.hpp"

class Get: public Methods
{
    private:
		std::string	_userAgent;
		std::string	_accept;
		std::pair<unsigned int, std::string> _findCodeMess(Server srv);

    public:
        Get(Request requ);
		const std::string	createResponse(Server srv);
};
#endif // !GET_HPP
