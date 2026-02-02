#ifndef INCLUDES_HPP

#define INCLUDES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <ctime>

#include "colors.hpp"
#include "Client.hpp"
#include "epoll/Epoll.hpp"
#include "socket/Socket.hpp"

std::time_t	getTimeNow();

#endif