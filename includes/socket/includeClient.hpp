#ifndef INCLUDEREQUEST_HPP

#define INCLUDEREQUEST_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>
int	receiveRequest(Config &conf, std::vector<Socket *> &sockets);
int	acceptClient(std::vector<Socket *> sockets, std::vector<Socket *>::iterator &itSock, std::vector<SocketData *>::iterator &itSD);

#endif