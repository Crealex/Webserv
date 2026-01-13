#include "../../includes.hpp"
#include "Envp.hpp"

class CGI {

	private:

		int _pipeFromCGI[2];
		int _pipeToCGI[2];

	public:

		CGI();
		CGI(const CGI& cpy);
		CGI& operator=(const CGI& src);
		~CGI();

};