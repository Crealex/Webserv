#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/requests/methodsClass.hpp"
#include "../includes/requests/ResponseError.hpp"

void sendResponse(Client &client, Server server)
{
	std::string response;
	bool ret;
	
	try
	{
		std::cout << MAGENTA << BOLD << "REQUESTS: " << RESET << std::endl;
		std::cout << client.getBuf() << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		Methods *request;
		request = createMethod((char *)client.getBuf().c_str(), server.getMaxSize(), ret);
		response = request->createResponse(server);
		delete request;
	}
	catch(ResponseError& e)
	{
		response = e.createResponse(server);
	}
	client.setKeepAlive(ret);
	std::cout << "response:" << response  << std::endl;
	while (send(client.getFdClient(), response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
}
