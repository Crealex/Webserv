#ifndef INCLUDESOCKET_HPP

#define INCLUDESOCKET_HPP

#include "../includes.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>

int	createSocket(Config conf);

#endif