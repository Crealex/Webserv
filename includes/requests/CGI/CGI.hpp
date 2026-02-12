#include "../../includes.hpp"
#include "../../epoll/Epoll.hpp"
#include "Envp.hpp"

#ifndef CGI_HPP
# define CGI_HPP

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
		Location*	_retRightLoc(std::string path, Server serv);
		inline std::string	_retExtension(std::string str);

	public:

		CGI();
		CGI(const CGI& cpy);
		CGI& operator=(const CGI& src);
		~CGI();

		const int	&getReadFD() const;
		
		int		getChildPid();
		char	*getResponse();
		bool	subprocessStarted();
		bool	subprocessExited();
		
		void	sendBody(std::string body);
		void	closeAllFd();
		void	checkSubprocess();
		void	reset();
		void	setEnvp(Client &client, Request &req);
		void	startSubprocess(const std::string path, const std::string interpreter);
		void	constructFD(Epoll &epoll);

		bool	isCGI(std::string path, Server server);
};

#endif