#include "../../../includes/requests/CGI/CGI.hpp"

CGI::CGI()
{
	if (pipe(_pipeFromCGI) != 0)
		throw std::runtime_error("Error, could not create pipe from CGI");
	// set les 2 pipe en non blocking

	if (pipe(_pipeToCGI) != 0)
	{
		close(_pipeFromCGI[0]);
		close(_pipeFromCGI[1]);
		throw std::runtime_error("Error, could not create pipe to CGI");
	}

	// set les 2 pipe en non blocking
}

CGI::CGI(const CGI& cpy)
{
	
}

CGI &CGI::operator=(const CGI& src)
{
	if (this != &src)
	{
		
	}
	return *this;
}

CGI::~CGI()
{

}
