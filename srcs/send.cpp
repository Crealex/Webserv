#include "../includes/includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/requests/Request.hpp"
#include "../includes/requests/methodsClass.hpp"
#include "../includes/requests/ResponseError.hpp"

void sendResponse(Client *client, Server server)
{
	std::string response;
	Request req;
	
	try
	{
		std::cout << MAGENTA << BOLD << "REQUESTS: " << RESET << std::endl;
		std::cout << client->getBuf() << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		Methods *request;
		req = createRequest((char *)client->getBuf().c_str(), server.getMaxSize());

		request = createMethod(req);
		response = request->createResponse(server);
		delete request;
	}
	catch(ResponseError& e)
	{
		response = e.createResponse(server);
	}
	client->setKeepAlive(req._keepAlive);
	// std::cout << RED << BOLD << "SEND ARG: \n" << RESET
	// 	<< "fdclient = " << client.getFdClient()
	// 	<< "\nresponse " << response 
	// 	<< "\nsize = " << response.size() << std::endl;
	while (send(client->getFdClient(), response.c_str(), response.size(), 0) == -1)
	{
		std::cout << RED << "send failed, retry in processing" << RESET << std::endl;
	}
	std::cout << "TEEEEEEEEEEEST" << std::endl;
}
