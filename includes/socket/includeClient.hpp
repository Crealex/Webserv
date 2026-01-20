#ifndef INCLUDECLIENT_HPP

#define INCLUDECLIENT_HPP

#include "../includes.hpp"
#include "Socket.hpp"
#include "../Client.hpp"
#include "../epoll/Epoll.hpp"

#include <sys/socket.h>

void	handleClient(std::vector<Socket *> &sockets, std::vector<Server> servers, Epoll &epoll, std::vector<Client *> &clients);

#endif
