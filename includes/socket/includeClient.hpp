#ifndef INCLUDEREQUEST_HPP

#define INCLUDEREQUEST_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>

void	handleClient(std::vector<Socket *> &sockets, Config conf);

#endif