#ifndef INCLUDECLIENT_HPP

#define INCLUDECLIENT_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"
#include "../epoll/Epoll.hpp"

#include <sys/socket.h>

void	handleClient(std::vector<Socket *> &sockets, std::vector<Server> servers, Epoll &epoll);

#endif