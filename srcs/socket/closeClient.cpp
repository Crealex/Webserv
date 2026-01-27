#include "../../includes/includes.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/epoll/Epoll.hpp"

void	closeClient(std::vector<Client *> &clients, int idClient, Epoll &epoll)
{
	int		indexFdEPoll;
	int		sizeEpollEvents;
	bool	isExisting;

	indexFdEPoll = 0;
	sizeEpollEvents = epoll.getNbSockets();
	isExisting = false;
	std::cout << YELLOW << "size : " << sizeEpollEvents << ", " << std::boolalpha << isExisting << ", " << idClient << ", " << clients.size() << ", " << clients[idClient]->getFdClient() << std::endl << RESET;
	for (int i = 0; i < sizeEpollEvents; i++)
	{
		std::cout << "epoll : " << epoll.getEvents()[i].data.fd << std::endl;
		if (epoll.getEvents()[i].data.fd == clients[idClient]->getFdClient())
		{
			indexFdEPoll = i;
			isExisting = true;
			break ;
		}
	}
	if (!isExisting)
		return ;
	std::cout << "epoooooooll : " << epoll.getEvents()[indexFdEPoll].data.fd << std::endl;
	epoll_ctl(epoll.getEpollFd(), EPOLL_CTL_DEL, epoll.getEvents()[indexFdEPoll].data.fd, &epoll.getEvents()[indexFdEPoll]);
	epoll.setNbSockets(sizeEpollEvents - 1);

	if (clients[idClient]->getFdClient() > -1)
		close(clients[idClient]->getFdClient());
	delete clients[idClient];
	clients.erase(clients.begin() + idClient);
}
