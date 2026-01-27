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
	int counter = 0;

	nbClients = clients.size();
	if (nbClients == 0)
		return ;
	for (int i = 0; i < nbClients; i++)
	{
		if (clients[i]->checkTimeout())
		{
			std::cout << "hereeeee" << std::endl;
			closeClient(clients, i, epoll);
			nbClients = clients.size();
			i--;
			counter++;
			std::cout << "i : " << i << ", " << nbClients << std::endl;
			if (counter == 2)
				std::exit(1);
		}
	}
	std::cout << RED << "HERE" << std::endl << RESET; 
}