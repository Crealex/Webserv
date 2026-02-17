#ifndef GET_HPP
# define GET_HPP

# include "../includes.hpp"
# include "Method.hpp"

class Get: public Methods
{
    private:
		std::string	_userAgent;
		std::string	_accept;
		std::pair<unsigned int, std::string> _findCodeMess(const Server &srv);
		bool	_isAllowedAutoIndex(const Server &srv);

    public:
        Get(const Request &requ);
		const std::string	createResponse(const Server &srv);
};
#endif // !GET_HPP
