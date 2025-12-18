#ifndef INCLUDESEND_HPP

#define INCLUDESEND_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>

void sendResponse(unsigned int socket, char *buff, Config conf);

#endif
