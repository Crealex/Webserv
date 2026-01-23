#include "../../includes.hpp"
#include "../../epoll/Epoll.hpp"
#include "Envp.hpp"

class CGI {

	private:

		int		_pipeFromCGI[2];
		int		_pipeToCGI[2];
		int		_childPid;
		bool 	_started;
		bool	_exited;
		Envp	_env;

		void reconstruct(Epoll epoll);

	public:

		CGI(Epoll epoll);
		CGI(const CGI& cpy);
		CGI& operator=(const CGI& src);
		~CGI();

		int		getChildPid();
		char	*getResponse();
		bool	subprocessStarted();
		bool	subprocessExited();

		void closeAllFd();
		void checkSubprocess();
		void reset(Epoll epoll);
		void sendBody(std::string body);
		void setEnvp(Client client, Request req);
		void startSubprocess(const std::string path, const std::string interpreter);
};
