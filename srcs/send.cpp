#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/requests/methodsClass.hpp"
#include "../includes/requests/ResponseError.hpp"

void sendResponse(Client client, Server server)
{
	std::string response;
	
	try
	{
		Methods *request;
		request = createMethod(buff, serv.getMaxSize());
		response = request->createResponse(serv);
		delete request;
	}
	catch(ResponseError& e)
	{
		response = e.createResponse(serv);
	}
	
	std::cout << "response:" << response  << std::endl;
	while (send(socket, response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
}
