#include "../../../includes/requests/CGI/Envp.hpp"

Envp::Envp()
{
	
}

Envp::Envp(const Envp& cpy)
{
	_env.clear();
	_env = cpy.getVec();
}

Envp &Envp::operator=(const Envp& src)
{
	if (this != &src)
	{
		_env.clear();
		_env = src.getVec();
	}
	return *this;
}

Envp::~Envp()
{
	
}

void Envp::setEnv(std::string filename, std::string name, Request &req)
{
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("REQUEST_METHOD=" + req.getMethod());
	_env.push_back("REQUEST_URI=" + req.getURI());
	_env.push_back("SCRIPT_FILENAME=" + filename);
	_env.push_back("SCRIPT_NAME=" + name);
	_env.push_back("QUERY_STRING=" + req.getQuery());
	_env.push_back("CONTENT_TYPE=" + req.getContentType());
	_env.push_back("CONTENT_LENGTH=" + req.getStrContentLength());
	// _env.push_back("SERVER_NAME=" + conf.getAddressPort()[0].name);
	// _env.push_back("SERVER_PORT=" + getInStr(client.getSockadd().sin_port);
	_env.push_back("HTTP_ACCEPT=" + req.getAccept());
	_env.push_back("HTTP_HOST=" + req.getHost());
	_env.push_back("HTTP_LOCATION=" + req.getLocation());
	_env.push_back("HTTP_USER_AGENT=" + req.getUserAgent());
}

std::vector<std::string> Envp::getVec() const
{
	return _env;
}

char **Envp::getEnv() const
{
	char **ret = new char*[_env.size()];

	size_t i = 0;
	for (; i <= _env.size() - 1; i++)
	{
		ret[i] = const_cast<char *>(_env[i].c_str());
	}
	ret[i] = NULL;

	return ret;
}
