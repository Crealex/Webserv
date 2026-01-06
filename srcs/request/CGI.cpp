#include "../../includes/includes.hpp"

std::map<std::string, std::string> initMap()
{
	std::map<std::string, std::string> m;

	m["AUTH_TYPE"] = "AUTH_TYPE=null";
	m["CONTENT_LENGTH"] = "";
	m["CONTENT_TYPE"] = "";
	m["GATEWAY_INTERFACE"] = "";
	m["PATH_TRANSLATED"] = "";
	m["QUERY_STRING"] = "";
	m["REMOTE_ADDR"] = "";
	m["REMOTE_HOST"] = "";
	m["REMOTE_IDENT"] = "";
	m["REMOTE_USER"] = "";
	m["REQUEST_METHOD"] = "";
	m["SCRIPT_NAME"] = "";
	m["SERVER_NAME"] = "";
	m["SERVER_PORT"] = "";
	m["SERVER_PROTOCOL"] = "";
	m["SERVER_SOFTWARE"] = "";

	return m;
}

void CGI()
{
	std::map<std::string, std::string> envMap;



}
 