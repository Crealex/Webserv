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

void Envp::setEnv(Client &client, Request &req)
{
	(void)client;
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("REQUEST_METHOD=" + req._method);
	_env.push_back("REQUEST_URI=" + req._URI);
	_env.push_back("SCRIPT_FILENAME=");	// use Client to create those
	_env.push_back("SCRIPT_NAME=");		// use Client to create those
	_env.push_back("QUERY_STRING=" + req._query);
	_env.push_back("CONTENT_TYPE=" + req._ContentType);
	_env.push_back("CONTENT_LENGTH=" + req.getStrContentLength());
	// _env.push_back("SERVER_NAME=" + conf.getAddressPort()[0].name);
	// _env.push_back("SERVER_PORT=" + getInStr(client.getSockadd().sin_port);
	_env.push_back("HTTP_ACCEPT=" + req._accept);
	_env.push_back("HTTP_HOST=" + req._host);
	_env.push_back("HTTP_LOCATION=" + req._location);
	_env.push_back("HTTP_USER_AGENT=" + req._userAgent);
}

std::vector<std::string> Envp::getVec() const
{
	return _env;
}

char **Envp::getEnv() const
{
	char **ret = new char*[_env.size() + 1];

	size_t i = 0;
	for (; i <= _env.size(); i++)
	{
		ret[i] = (char *)_env[i].c_str();
	}
	ret[i] == NULL;

	return ret;
}
