#ifndef INCLUDES_HPP

#define INCLUDES_HPP

#include <iostream>

#include <map>
#include <string>
#include <vector>

#include <exception>
#include <stdexcept>

#include <fstream>
#include <sstream>

#include <cstddef>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <utility>

#include "colors.hpp"

class Socket;
class Server;

void					sockOptNonBlocking(int &socketFd);
std::vector<Socket *>	createSocket(std::vector<Server> srvs, int &nbSockets);

#endif