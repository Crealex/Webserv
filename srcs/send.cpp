#include "../includes/includes.hpp"
#include "../includes/socket/Socket.hpp"
#include "../includes/socket/SocketData.hpp"
#include "../includes/requests/methodsClass.hpp"
#include "../includes/requests/ResponseError.hpp"

void sendResponse(unsigned int socket, char *buff, Server)
{
	std::string response;
	
	try
	{
		Methods *request;
		request = createMethod(buff, conf.getMaxSize());
		response = request->createResponse(conf);
		delete request;
	}
	catch(ResponseError& e)
	{
		response = e.createResponse(conf);
	}
	
	std::cout << "response:" << response  << std::endl;
	while (send(socket, response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
}
