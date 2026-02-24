#include "../includes/printDebug.hpp"
#include "../includes/includes.hpp"
#include "../includes/Loop.hpp"

static void	isDuplicateServer(Server temp, std::vector<Server> res)
{
	int	sizeRes;
	int	sizeAddPort;
	int	sizeAPTemp;

	sizeRes = res.size();
	for (int i = 0; i < sizeRes; i++)
	{
		sizeAddPort = res[i].getAddressPort().size();
		for (int j = 0; j < sizeAddPort; j++)
		{
			sizeAPTemp = temp.getAddressPort().size();
			for (int k = 0; k < sizeAPTemp; k++)
			{
				if (res[i].getAddressPort()[j].first == temp.getAddressPort()[k].first && res[i].getAddressPort()[j].second == temp.getAddressPort()[k].second)
					throw std::invalid_argument(RED "Error : an add/port couple already exists with another hostname" RESET);
			}
		}
		if (res[i].getHostname() == temp.getHostname())
			throw std::invalid_argument(RED "Error : this server exists already" RESET);
	}
}

static std::vector<Server>	createServers(std::string path)
{
	std::vector<Server>	res;
	std::vector<server>	structServers;
	int					sizeStructSrv;

	structServers = createVectStructSrv(path);
	sizeStructSrv = structServers.size();
	for (int i = 0; i < sizeStructSrv; i++)
	{
		Server	temp(structServers[i]);
		isDuplicateServer(temp, res);
		res.push_back(temp);
	}
	return (res);
}

int main (int argc, char **argv) 
{
	std::vector<Server>		servers;

	if (argc > 2)
	{
		std::cerr << RED << "Invalid number of arguments, you need the executable and maybe a config file" << RESET << std::endl;
		return (-1);
	}
	try 
	{
		servers = createServers(argv[1]);
		Loop	loop(servers);
		loop.runLoop();
	}
	catch (std::exception &e) 
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	std::cout << RED << "help end" << RESET << std::endl ;
	return 0;
}
