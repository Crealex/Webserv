#include "../../../includes/requests/CGI/CGI.hpp"
#include <sys/wait.h>
#include <cstdlib>

CGI::CGI() : _started(false), _exited(false)
{
	_pipeFromCGI[0] = -1;
	_pipeFromCGI[1] = -1;
	_pipeToCGI[0] = -1;
	_pipeToCGI[1] = -1;
}

CGI::CGI(const CGI& cpy)
{
	_env = cpy._env;
}

CGI &CGI::operator=(const CGI& src)
{
	if (this != &src)
	{
		_env = src._env;
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

void	CGI::_sockOptNonBlocking(int &socketFd)
{
		int	opt;

		opt = 1;
		::fcntl(socketFd, F_SETFL, O_NONBLOCK);
		::setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
}

void CGI::setEnvp(Server &serv, Request &req)
{
	std::string name;
	std::string filename;

	name = req.getLocation();
	if (name.find('.') == std::string::npos)
	{
		for (size_t i = 0; i < serv.getLocations().size(); i++)
		{
			if (serv.getLocations()[i].getPath() == name)
			{
				name += '/' + serv.getLocations()[i].getIndex();
				break ;
			}
		}
	}
	filename = serv.getRoot() + name;

	_env.setEnv(filename, name, req);
}

void CGI::constructFD()
{
	if (::pipe(_pipeFromCGI) != 0)
		throw std::runtime_error("Error, could not create pipe from CGI");
	_sockOptNonBlocking(_pipeFromCGI[0]);
	_sockOptNonBlocking(_pipeFromCGI[1]);
	if (::pipe(_pipeToCGI) != 0)
	{
		::close(_pipeFromCGI[0]);
		::close(_pipeFromCGI[1]);
		_pipeFromCGI[0] = -1;
		_pipeFromCGI[1] = -1;
		throw std::runtime_error("Error, could not create pipe to CGI");
	}
	_sockOptNonBlocking(_pipeToCGI[0]);
	_sockOptNonBlocking(_pipeToCGI[1]);
}

void CGI::reset()
{
	if (_started && !_exited)
		::kill(_childPid, SIGKILL);
	
	_env = Envp();

	_childPid = 0;
	_started = false;
	_exited = false;
	closeAllFd();
}

void CGI::closeAllFd()
{
	if (_pipeFromCGI[0] != -1)
	{
		::close(_pipeFromCGI[0]);
		_pipeFromCGI[0] = -1;
	}
	if (_pipeFromCGI[1] != -1)
	{
		::close(_pipeFromCGI[1]);
		_pipeFromCGI[1] = -1;
	}
	if (_pipeToCGI[0] != -1)
	{
		::close(_pipeToCGI[0]);
		_pipeToCGI[0] = -1;
	}
	if (_pipeToCGI[1] != -1)
	{
		::close(_pipeToCGI[1]);
		_pipeToCGI[1] = -1;
	}
}

void CGI::startSubprocess(const std::string path, const std::string interpreter)
{
	int pid = ::fork();
	if (pid == -1)
		throw std::runtime_error("Error, subprocess cannot be created");

	if (pid == 0)
	{
		::close(_pipeToCGI[1]);
		::dup2(_pipeToCGI[0], STDIN_FILENO);
		::close(_pipeToCGI[0]);
		::close(_pipeFromCGI[0]);
		::dup2(_pipeFromCGI[1], STDOUT_FILENO);
		::close(_pipeFromCGI[1]);

		char **args = new char*[3];
		args[0] = const_cast<char *>(interpreter.c_str());
		args[1] = const_cast<char *>(path.c_str());
		args[2] = NULL;

		char **env = _env.getEnv();
		if (::execve(args[0], args, env) == -1)
		{
			delete[] env;
			delete[] args;
			::exit(1);
		}
	}
	else
	{
		_started = true;
		_childPid = pid;
		::close(_pipeToCGI[0]);
		::close(_pipeFromCGI[1]);
		_pipeFromCGI[1] = -1;
		_pipeToCGI[0] = -1;
	}
}

void CGI::sendBody(std::string body)
{
	if (_started && !body.empty())
	{
		::write(_pipeToCGI[1], body.c_str(), body.size());
		::close(_pipeToCGI[1]);
		_pipeToCGI[1] = -1;
	}
}

std::string CGI::getResponse()
{
	std::string ret;

	if (_exited)
	{
		char buff[1024];
		int byteread;
		while ((byteread = ::read(_pipeFromCGI[0], buff, 1024)) != 0)
		{
			ret.append(buff, byteread);
		}
		if (byteread == -1)
			throw std::logic_error("Error dans read");
		::close(_pipeFromCGI[0]);
		_pipeFromCGI[0] = -1;
	}

	return ret;
}

void CGI::checkSubprocess(Request &req)
{
	if (_started)
	{
		int status;
		int ret = ::waitpid(_childPid, &status, WNOHANG);
		if (ret == -1)
			throw ResponseError(500, "Couldn't wait the CGI process", req);
		if (ret == 0)
			return;
		if (!WIFEXITED(status) || WEXITSTATUS(status))
			throw ResponseError(500, "CGI subprocess didn't normally exited", req);
		_exited = true;
	}
}

inline std::string CGI::_retExtension(std::string str)
{
	size_t pos = str.find('.');
	if (pos == std::string::npos)
		pos = 0;
	return str.substr(pos, str.size() - (pos));
}

Location CGI::_retRightLoc(std::string path, Server serv)
{
	std::string locStr = path.substr(0, path.find('.'));
	std::vector<Location> v = serv.getLocations();
	for (std::vector<Location>::iterator it = v.begin();
		it != v.end(); it++)
	{
		if (locStr == (*it).getPath())
			return (*it);	// return the adress of the object 'it' point to :D
	}

	return Location();
}

bool CGI::_cmpExt(std::string ext, std::map<std::string, std::string> map)
{
	std::map<std::string, std::string>::iterator it;
	for (it = map.begin(); it != map.end(); it++)
	{
		if (ext == it->first)
			return true;
	}
	return false;
}

bool CGI::isCGI(std::string path, Server server)
{
	std::string ext = _retExtension(path);
	if (ext == path)
	{
		Location loc = _retRightLoc(path, server);
		if (!loc.getIndex().empty())
		{
			std::string indexExt = _retExtension(loc.getIndex());
			return _cmpExt(indexExt, loc.getCgiHandler());
		}
		else
			return false; // TODO maybe throw ?
	}
	else
	{
		std::string substring = path.substr(0, path.find_last_of('/'));
		Location loc = _retRightLoc(substring, server);
		if (_cmpExt(ext, loc.getCgiHandler()))
		{
			std::ifstream is(std::string(server.getRoot() + path).c_str());
			
			if (is.is_open())
				return true;
			return false;
		}
		else
			return false;
	}
}

const int &CGI::getReadFD() const
{
	return _pipeFromCGI[0];
}
