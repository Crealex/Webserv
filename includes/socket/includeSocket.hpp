#ifndef INCLUDESOCKET_HPP

#define INCLUDESOCKET_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>

std::vector<Socket *>	createSocket(Config conf);
void	printSocketListen(std::vector<Socket *> sockets);

#endif