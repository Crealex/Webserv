#include "../../../includes/includes.hpp"
#include "../../../includes/Server.hpp"
#include "../../../includes/requests/Request.hpp"

static std::string retExtention(std::string str)
{
	size_t pos = str.find('.', 0);
	// if (pos == std::string::npos)
	// 	throw std::invalid_argument("Error, no extension");

	std::string ret(str, pos + 1, str.size() - pos - 1);

	return ret;
}

bool isCGI(Request req, Server serv)
{

}
