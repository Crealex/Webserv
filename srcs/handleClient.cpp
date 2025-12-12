#include "../includes/includes.hpp"
#include "../includes/Socket.hpp"

int	receiveRequest(Config &conf, std::vector<Socket *> &sockets)
{
	size_t	sizeRecv;

	sizeRecv = recv()
	return (0);
}

int	acceptClient(std::vector<Socket *> &sockets, std::vector<Socket *>::iterator &itSock, std::vector<SocketData *>::iterator &itSD)
{
	int fdClient;

	fdClient = 0;
	fdClient = accept((*itSD)->getFdClient(), nullptr, nullptr);
	if (fdClient < 0)
	{
		(*itSock)->eraseSocket(itSD);
		return (-1);
	}
	return (0);
}

int	handleClient(std::vector<Socket *> &sockets, Config conf)
{
	for (std::vector<Socket *>::iterator itSock = sockets.begin(); itSock != sockets.end(); itSock++)
	{
		for (std::vector<SocketData *>::iterator itSD = (*itSock)->getBegin(); itSD != (*itSock)->getEnd(); itSD++)
		{
			if (acceptClient(sockets, itSock, itSD) < 0)
				
		}
	}
	return (0);
}