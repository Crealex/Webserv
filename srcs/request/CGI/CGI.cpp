#include "../../../includes/requests/CGI/CGI.hpp"
#include <sys/wait.h>

CGI::CGI(Epoll epoll) : _started(false), _exited(false)
{
	if (pipe(_pipeFromCGI) != 0)
		throw std::runtime_error("Error, could not create pipe from CGI");
	sockOptNonBlocking(_pipeFromCGI[0]);
	sockOptNonBlocking(_pipeFromCGI[1]);
	epoll.addEpollFd(_pipeFromCGI[0], EPOLLOUT);
	epoll.addEpollFd(_pipeFromCGI[1], EPOLLIN);
	if (pipe(_pipeToCGI) != 0)
	{
		close(_pipeFromCGI[0]);
		close(_pipeFromCGI[1]);
		_pipeFromCGI[0] = -1;
		_pipeFromCGI[1] = -1;
		throw std::runtime_error("Error, could not create pipe to CGI");
	}
	sockOptNonBlocking(_pipeToCGI[0]);
	sockOptNonBlocking(_pipeToCGI[1]);
	epoll.addEpollFd(_pipeToCGI[0], EPOLLOUT);
	epoll.addEpollFd(_pipeToCGI[1], EPOLLIN);
}

CGI::CGI(const CGI& cpy)
{
	
}

CGI &CGI::operator=(const CGI& src)
{
	if (this != &src)
	{
		
	}
	return *this;
}

CGI::~CGI()
{
	closeAllFd();
}

int CGI::getChildPid()
{
	return _childPid;
}

bool CGI::subprocessStarted()
{
	return _started;
}

bool CGI::subprocessExited()
{
	return _exited;
}

void CGI::setEnvp(Client client, Request req)
{
	_env.setEnv(client, req);
}

void CGI::reconstruct(Epoll epoll)
{
	if (pipe(_pipeFromCGI) != 0)
		throw std::runtime_error("Error, could not create pipe from CGI");
	sockOptNonBlocking(_pipeFromCGI[0]);
	sockOptNonBlocking(_pipeFromCGI[1]);
	epoll.addEpollFd(_pipeFromCGI[0], EPOLLOUT);
	epoll.addEpollFd(_pipeFromCGI[1], EPOLLIN);
	if (pipe(_pipeToCGI) != 0)
	{
		close(_pipeFromCGI[0]);
		close(_pipeFromCGI[1]);
		_pipeFromCGI[0] = -1;
		_pipeFromCGI[1] = -1;
		throw std::runtime_error("Error, could not create pipe to CGI");
	}
	sockOptNonBlocking(_pipeToCGI[0]);
	sockOptNonBlocking(_pipeToCGI[1]);
	epoll.addEpollFd(_pipeToCGI[0], EPOLLOUT);
	epoll.addEpollFd(_pipeToCGI[1], EPOLLIN);
}

void CGI::reset(Epoll epoll)
{
	if (_started && !_exited)
		kill(_childPid, SIGKILL);
	
	_env = Envp();

	_childPid = 0;
	closeAllFd();
	reconstruct(epoll);
}

void CGI::closeAllFd()
{
	if (_pipeFromCGI[0] != -1)
	{
		close(_pipeFromCGI[0]);
		_pipeFromCGI[0] = -1;
	}
	if (_pipeFromCGI[1] != -1)
	{
		close(_pipeFromCGI[1]);
		_pipeFromCGI[1] = -1;
	}
	if (_pipeToCGI[0] != -1)
	{
		close(_pipeToCGI[0]);
		_pipeToCGI[0] = -1;
	}
	if (_pipeToCGI[1] != -1)
	{
		close(_pipeToCGI[1]);
		_pipeToCGI[1] = -1;
	}
}

void CGI::startSubprocess(const std::string path, const std::string interpreter)
{
	int pid = fork();
	if (pid == -1)
		throw std::runtime_error("Error, subprocess cannot be created");

	if (pid == 0)
	{
		close(_pipeToCGI[1]);
		dup2(_pipeToCGI[0], STDIN_FILENO);
		close(_pipeToCGI[0]);
		close(_pipeFromCGI[0]);
		dup2(_pipeFromCGI[1], STDOUT_FILENO);
		close(_pipeFromCGI[1]);

		char **args = new char*[3];
		args[0] = const_cast<char *>(interpreter.c_str());
		args[1] = const_cast<char *>(path.c_str());
		args[2] = NULL;

		char **env = _env.getEnv();
		if (execve(args[0], args, env) == -1)
		{
			delete env;
			delete args;
			throw std::runtime_error("Error, could'nt create subprocess");
		}
	}
	else 
	{
		_started = true;
		_childPid = pid;
		close(_pipeToCGI[0]);
		close(_pipeFromCGI[1]);
		_pipeFromCGI[1] = -1;
		_pipeToCGI[0] = -1;
	}
}

void CGI::sendBody(std::string body)
{
	if (_started)
	{	// send pack by pack the body to not overload the fd
		write(_pipeToCGI[1], body.c_str(), body.size());
		close(_pipeToCGI[1]);
		_pipeToCGI[1] = -1;
	}
}

char *CGI::getResponse()
{
	std::string ret;

	if (_exited)
	{
		char buff[1024];
		while (read(_pipeFromCGI[0], buff, 1024) != 0)
		{
			ret += buff;
		}
		close(_pipeFromCGI[0]);
		_pipeFromCGI[0] = -1;
	}

	return const_cast<char *>(ret.c_str());
}

void CGI::checkSubprocess()
{
	if (_started)
	{
		int status;
		int ret = waitpid(_childPid, &status, WNOHANG);
		if (ret == -1)
			throw std::runtime_error("Error, waitpid could'nt wait subprocess");
		if (ret == 0)
			return;
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			throw std::runtime_error("Error, subprocess exited non normally");
		_exited = true;
	}
}
