#ifndef DELETE_HPP
# define DELETE_HPP

# include "../includes.hpp"
# include "methodsClass.hpp"

class Delete: public Methods
{
    private:
		
    public:
        Delete(Request requ); 
		const std::string createResponse(Config conf);
};
#endif // !DELETE_HPP
