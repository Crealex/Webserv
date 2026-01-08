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
	_env.push_back("REQUEST_URI=");		// find where this is
	_env.push_back("SCRIPT_FILENAME");	// find where this is
	_env.push_back("SCRIPT_NAME");		// find where this is
	_env.push_back("QUERY_STRING");		// what is after '?' in the URI
	_env.push_back("CONTENT_TYPE=" + req._ContentType);
	_env.push_back("CONTENT_LENGTH=" + req.getStrContentLength());
	_env.push_back("SERVER_NAME=" + conf.getAddressPort()[0].name);
	_env.push_back("SERVER_PORT=" + getInStr(conf.getAddressPort()[0].addressPort[0].second));
	if (!req._accept.empty())	
		_env.push_back("HTTP_ACCEPT=" + req._accept);
	if (!req._host.empty())	
		_env.push_back("HTTP_HOST=" + req._host);
	if (!req._location.empty())
		_env.push_back("HTTP_LOCATION" + req._location);
	if (!req._userAgent.empty())
		_env.push_back("HTTP_USER_AGENT=" + req._userAgent);
}
