#include "../../includes.hpp"
#include "../Request.hpp"
#include "../../Config.hpp"

class Envp {

	private:

		std::vector<std::string> _env;

	public:

		Envp();
		Envp(const Envp& cpy);
		Envp& operator=(const Envp& src);
		~Envp();

		void	setEnv(Config conf, Request req);
		
		char**	getEnv() const;
		std::vector<std::string> getVec() const;
};