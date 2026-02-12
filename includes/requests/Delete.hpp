#ifndef DELETE_HPP
# define DELETE_HPP

# include "../includes.hpp"
# include "Method.hpp"

class Delete: public Methods
{
    private:
		
    public:
        Delete(const Request &requ); 
		const std::string createResponse(const Server &srv);
};
#endif // !DELETE_HPP
