#include "../../includes/includes.hpp"
#include "../../includes/epoll/Epoll.hpp"
#include "../../includes/socket/includeSocket.hpp"
#include <fcntl.h>

void	sockOptNonBlocking(int &socketFd)
{
		int	opt;

		opt = 1;
		::fcntl(socketFd, F_SETFL, O_NONBLOCK);
		::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}
