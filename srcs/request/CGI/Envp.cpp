#include "../../../includes/requests/CGI/Envp.hpp"

Envp::Envp()
{
	
}

Envp::Envp(const Envp& cpy)
{
	_env = cpy.getVec();
}

Envp &Envp::operator=(const Envp& src)
{
	if (this != &src)
	{
		_env = src.getVec();
	}
	return *this;
}

Envp::~Envp()
{
	
}

static std::string getInStr(size_t n)
{
	std::stringstream ss;
	std::string ret;

	ss << n;
	ss >> ret;

	return ret;
}

void Envp::setEnv(Config conf, Request req)
{
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("REQUEST_METHOD=" + req._method);
	_env.push_back("REQUEST_URI=" + req._URI);
	_env.push_back("SCRIPT_FILENAME=");	// use Client to create those
	_env.push_back("SCRIPT_NAME=");		// use Client to create those
	_env.push_back("QUERY_STRING=" + req._query);
	_env.push_back("CONTENT_TYPE=" + req._ContentType);
	_env.push_back("CONTENT_LENGTH=" + req.getStrContentLength());
	_env.push_back("SERVER_NAME=" + conf.getAddressPort()[0].name);
	_env.push_back("SERVER_PORT=" + getInStr(conf.getAddressPort()[0].addressPort[0].second));
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

	for (int i = 0; i <= _env.size(); i++)
	{
		ret[i] = (char *)_env[i].c_str();
	}

	return ret;
}
