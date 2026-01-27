#include "../includes/Loop.hpp"

// Constructor & Destructor

Loop::Loop()
{

}

Loop::~Loop()
{

}

// METHODS
// PUBLIC


void	Loop::runLoop()
{
	while (1)
	{
		int					epollCounterWait;
		int					idClient;
		std::string			hostnameOfSrvSock;
		epoll_event			events[epoll.getNbSockets()];

		epollCounterWait = 0;
		idClient = 0;
	
		epollCounterWait = ::epoll_wait(epoll.getEpollFd(), events, epoll.getNbSockets(), 2000);
		std::cout << GREEN << "dana : " << epollCounterWait << std::endl << RESET;
		checkAllTimeout(clients, epoll);
		if (epollCounterWait < 1)
			continue ;
		for (int indexEvent = 0; indexEvent < epollCounterWait; indexEvent++)
		{
			if (events[indexEvent].events & EPOLLIN && isServerSocket(events[indexEvent].data.fd, sockets, hostnameOfSrvSock))
			{
				std::cout << "Just before accept : " << epollCounterWait << std::endl;
			
				if (acceptClient(events[indexEvent].data.fd, clients, epoll, hostnameOfSrvSock) < 0)
					continue ;
				for (size_t i = 0; i < clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << clients[i]->getFdClient() << ", " << clients[i]->getHostname() << ", keep alive : " << clients[i]->getKeepAlive() << std::endl << RESET;
				}
				std::cout << "epoll size : " << epoll.getNbSockets() << std::endl;
			}
			std::cout << GREEN << "epollin : " << events[indexEvent].events << ", " << (isClientSocket(events[indexEvent].data.fd, clients, idClient)) << std::endl << RESET;
			if (events[indexEvent].events & EPOLLIN && isClientSocket(events[indexEvent].data.fd, clients, idClient))
			{
				std::cout << "receive" << std::endl;
				if (receiveRequest(clients[idClient], epoll) == 0)
				{
					std::cout << LIGHT_YELLOW << "Request : " << clients[idClient]->getBuf() << std::endl << RESET;
					closeClient(clients, idClient, epoll);
					continue ;
				}
				std::cout << MAGENTA <<"Request : " << clients[idClient]->getBuf() << std::endl << RESET;
			}
			// std::cout << "event : " << events[indexEvent].data.fd << ", " << events[indexEvent].events << ", " << isClientSocket(events[indexEvent].data.fd, clients, idClient) << std::endl;
			clients[idClient]->checkTimeoutRequest();
			if (events[indexEvent].events & EPOLLOUT && isClientSocket(events[indexEvent].data.fd, clients, idClient))
			{
				std::cout << "chez kilian" << std::endl;
				std::cout << RED << "Request : " << clients[idClient]->getBuf() << std::endl << RESET;
				std::cout << "maybe after" << std::endl;
				sendResponse(clients[idClient], goodServer(clients[idClient], servers));
				// std::cout << BLUE << "after send : " << clients[idClient]->getKeepAlive() << std::endl << RESET;
				for (size_t i = 0; i < clients.size(); i++)
				{
					std::cout << BLUE << "clients : " << clients[i]->getFdClient() << ", " << clients[i]->getHostname() << ", keep alive : " << clients[i]->getKeepAlive() << std::endl << RESET;
				}
				if (clients[idClient]->getKeepAlive() == false)
					closeClient(clients, idClient, epoll);
				else
				{
					clients[idClient]->resetClient();
					epoll.setEvents(clients[idClient], EPOLLIN);
				}
				std::cout << "end" << std::endl;
			}
		}
	}
}