#ifndef DELETE_HPP

# define DELETE_HPP
# include "../includes.hpp"
#include "methodsClass.hpp"

struct Request;

class Delete: public Methods
{
    private:
		
    public:
        Delete(request requ); 
		const std::string createResponse();
};
#endif // !DELETE_HPP
