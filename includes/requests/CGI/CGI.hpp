#include "../../includes.hpp"
#include "../../epoll/Epoll.hpp"
#include "../../requests/ResponseError.hpp"
#include "Envp.hpp"

#ifndef CGI_HPP
# define CGI_HPP


class Epoll;

class CGI {

	private:

		int			_pipeFromCGI[2];
		int			_pipeToCGI[2];
		int			_childPid;
		bool 		_started;
		bool		_exited;
		Envp		_env;
		std::string	_body;

		bool		_cmpExt(std::string ext, std::map<std::string, std::string> map);
		void		_sockOptNonBlocking(int &socketFd);		
		Location	_retRightLoc(std::string path, Server serv);
		std::string	_retExtension(std::string str);

	public:

		CGI();
		CGI(const CGI& cpy);
		CGI& operator=(const CGI& src);
		~CGI();

		const int	&getReadFD() const;
		
		int			getChildPid();
		bool		subprocessStarted();
		bool		subprocessExited();
		std::string	getResponse();
		
		void	sendBody(std::string body);
		void	closeAllFd();
		void	checkSubprocess(Request &req);
		void	reset();
		void	setEnvp(Server &serv, Request &req);
		void	startSubprocess(const std::string path, const std::string interpreter);
		void	constructFD(Epoll &epoll);

		bool	isCGI(std::string path, Server server);
};

#endif