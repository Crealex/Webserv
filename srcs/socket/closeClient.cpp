#include "../../includes/includes.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/epoll/Epoll.hpp"

void	closeClient(std::vector<Client *> &clients, int idClient, Epoll &epoll)
{
	epoll_ctl(epoll.getEpollFd(), EPOLL_CTL_DEL, clients[idClient]->getFdClient(), 0);
	epoll.setNbSockets(epoll.getNbSockets() - 1);

	if (clients[idClient]->getFdClient() > -1)
		close(clients[idClient]->getFdClient());
	delete clients[idClient];
	clients.erase(clients.begin() + idClient);
}
