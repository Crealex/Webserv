#include "../../includes.hpp"
#include "../Request.hpp"

#ifndef ENVP_HPP
# define ENVP_HPP

class Envp {

	private:

		std::vector<std::string> _env;

	public:

		Envp();
		Envp(const Envp& cpy);
		Envp& operator=(const Envp& src);
		~Envp();

		void	setEnv(std::string filename, std::string name, Request &req);
		
		char**	getEnv() const;
		std::vector<std::string> getVec() const;
};

#endif