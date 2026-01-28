#ifndef INCLUDESEND_HPP

#define INCLUDESEND_HPP

#include "../includes.hpp"
#include "../epoll/Epoll.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "Socket.hpp"
#include "includeSockNonBlocking.hpp"

#include <sys/socket.h>

void	sendResponse(Client *client, Server server);

#endif
