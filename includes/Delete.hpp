#ifndef DELETE_HPP

# define DELETE_HPP
# include "includes.hpp"

struct request;

class Delete
{
    private:
		
    public:
        Delete(request requ); 
		const std::string createResponse();
};
#endif // !DELETE_HPP
