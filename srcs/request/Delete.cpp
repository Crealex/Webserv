
#include "../../includes/requests/Delete.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"

Delete::Delete(Request requ): Methods(requ)
{
    std::cout << GREEN << "Default Delete constructor called" << RESET << std::endl;
}

const std::string Delete::createResponse()
{
	
}
