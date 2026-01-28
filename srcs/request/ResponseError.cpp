#include "../../includes/requests/ResponseError.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include "../../includes/requests/Request.hpp"
#include "../../includes/colors.hpp"

ResponseError::ResponseError(unsigned int code, std::string message, Request requ): std::exception(), Methods(requ), _code(code), _message(message)
{
	//std::cout << LIGHT_RED << "DEBUG: Created exception error response" << RESET << std::endl;
}

ResponseError::~ResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
}

const std::string ResponseError::createResponse(Server srv)
{
	std::string resp;
	std::string path;
	std::stringstream ss;
	std::ifstream file;
	Request dataError;

	ss << this->_code;
	path = srv.getRoot() + "/error/" + ss.str() + ".html";
	file.open(path.c_str());
	std::cout << "code: " << this->_code << std::endl;
	if (!file.is_open())
	{
		path = srv.getRoot() + "/error/default.html";
		file.open(path.c_str());
	}
	std::cout << "path in ErrorResponse: " << path << std::endl;

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	char* buffer = new char[size];
	file.read(buffer, size);
	std::string fileStr(buffer, size);
	delete[] buffer;

	std::cout << BOLD << YELLOW << "HEEEERRRREEEEEEEE" << std::endl;
	if (!addContentType(&resp, "text/html", path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path))
		throw ResponseError(500, "can't add content length", dataError);
	addStartLine(&resp, this->_protocol, this->_code, this->_message);
	resp.append("\r\n\r\n");
	addBody(&resp, fileStr);
	std::cout << "resp in error response: " << resp << std::endl;
	return (resp);
	// std::cout << "bonjour jespere ca marche" << std::endl;
}

