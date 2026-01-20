#ifndef INCLUDESEND_HPP

#define INCLUDESEND_HPP

#include "../includes.hpp"
#include "../epoll/Epoll.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "Socket.hpp"
#include "includeSockNonBlocking.hpp"

#include <sys/socket.h>

void	addEpollFd(int fd, int nbPollFd, Epoll &epoll, uint32_t event);
void	sendResponse(Client *client, Server server);

#endif
