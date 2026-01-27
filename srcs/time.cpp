#include "../includes/includes.hpp"
#include "../includes/socket/includeSend.hpp"
#include <cstdlib>

std::time_t	getTimeNow()
{
	time_t	timestamp;
	
	time(&timestamp);
	return (timestamp);
}

void	checkAllTimeout(std::vector<Client *> &clients, Epoll &epoll)
{
	int	nbClients;

	nbClients = clients.size();
	if (nbClients == 0)
		return ;
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i]->checkTimeout())
		{
			closeClient(clients, i, epoll);
			nbClients = clients.size();
			i--;
		}
	}
}