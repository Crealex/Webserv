#include "../includes/includes.hpp"
#include "../includes/Config.hpp"
#include "../includes/Socket.hpp"

void	createSocket(Config conf)
{
	std::vector<Socket>	sockets;
	size_t				sizeAddPort;
	int					checkFail;

	sizeAddPort = conf.getAddressPort().size();
	sockets.reserve(sizeAddPort);
	for (size_t i = 0; i < sizeAddPort; i++)
	{
		Socket	temp(conf, i);
		sockets.push_back(temp);
	}
	for (size_t i = 0; i < sizeAddPort; i++)
	{
		size_t	nbSocket;

		nbSocket = sockets[i].getSockaddrs().size();
		for (size_t j = 0; j < nbSocket; j++)
		{
			checkFail = bind(sockets[i].getSockaddrs()[j]);
		}
	}
}