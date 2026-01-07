#ifndef INCLUDESEND_HPP

#define INCLUDESEND_HPP

#include "../includes.hpp"
#include "../epoll/Epoll.hpp"
#include "../Config.hpp"
#include "Socket.hpp"

#include <sys/socket.h>

void	addEpollFd(int fd, int nbPollFd, Epoll epoll);
void	sendResponse(unsigned int socket, char *buff, unsigned int maxSize);

#endif