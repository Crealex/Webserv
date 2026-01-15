#include "../includes/Server.hpp"
#include "../includes/printDebug.hpp"
#include "../includes/includes.hpp"
#include "../includes/socket/includeSocket.hpp"
#include "../includes/socket/includeClient.hpp"

static bool	isDuplicateServer(Server temp, std::vector<Server> res)
{
	int	sizeRes;

	sizeRes = res.size();
	for (int i = 0; i < sizeRes; i++)
	{
		if (res[i].getHostname() == temp.getHostname())
			return (true);
	}
	return (false);
}

static std::vector<Server>	createServers(std::string path)
{
	std::vector<Server>	res;
	int					sizeStructSrv;
	std::vector<server>	structServers;

	structServers = createVectStructSrv(path);
	sizeStructSrv = structServers.size();
	for (int i = 0; i < sizeStructSrv; i++)
	{
		Server	temp(structServers[i]);
		if (isDuplicateServer(temp, res))
			throw std::invalid_argument(RED "Error : this server exists already" RESET);
		res.push_back(temp);
	}
	return (res);
}

int main (int argc, char **argv) 
{
	std::vector<Socket *>	sockets;
	std::vector<Server>		servers;
	int						nbSockets;
	if (argc > 2)
	{
		std::cerr << RED << "Invalid number of arguments, you need the executable and maybe a config file" << RESET << std::endl;
		return (-1);
	}
	try 
	{

		servers = createServers(argv[1]);
		sockets = createSocket(servers, nbSockets);
		if (sockets.size() == 0)
		{
			std::cerr << RED << "Error : no socket for the webserv" << std::endl << RESET;
			return (-2);
		}
		printSocketListen(sockets);
		Epoll	epoll(sockets, nbSockets);
		while (1)
			handleClient(sockets, servers, epoll);
	}
	catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	return 0;
}
