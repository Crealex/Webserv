#include "../includes/includes.hpp"
#include "../includes/Socket.hpp"
#include "../includes/SocketData.hpp"
#include "request/getMethods.cpp"

void sendResponse(unsigned int socket, Config conf, char *buff)
{
	Methods *request;
	std::string response;

	try
	{
		request = createMethod(buff);
		response = request->createResponse();
		delete request;
	}
	catch(const ResponseError& e)
	{
		response = e.createResponse();
	}
	
	while (send(socket, response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
}