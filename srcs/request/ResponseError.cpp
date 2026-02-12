#include "../../includes/requests/ResponseError.hpp"
#include <cstddef>
#include <exception>
#include <fstream>
#include <sstream>
#include "../../includes/requests/Request.hpp"
#include "../../includes/colors.hpp"

ResponseError::ResponseError(unsigned int code, const std::string &message, const Request &requ): std::exception(), Methods(requ), _code(code), _message(message)
{
	//std::cout << LIGHT_RED << "DEBUG: Created exception error response" << RESET << std::endl;
}

ResponseError::~ResponseError() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
}

std::string createFileStr(std::ifstream &file)
{
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	char* buffer = new char[size];
	file.read(buffer, size);
	std::string fileStr(buffer, size);
	delete[] buffer;

	return fileStr;
}

/**
 * @brief Building the html error pages when there is not specified
 *
 * @param code The error code
 * @param mess The error message
 * @return A string with the entire code of html error page
 */
std::string createBodyHTML(unsigned int code, std::string mess)
{
	std::stringstream ss;


	ss << "<!doctype html>\n <html lang=\"en\">\n<head>\n \
			<meta charset=\"UTF-8\" />\n \
			<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n \
			<title>Error " << code << "</title> \n \
			<style> \
				body,html {\n \
			font-family: \"Consolas\", monospace, Cursive;\n \
			width: 100%;\n \
			height: 100%;\n \
			background: #282a36;\n \
			margin: 0; \n\
			padding: 0; \n\
  			box-sizing: border-box;\n \
  			display: flex; \n\
  			flex-direction: column;\n \
  			color: #f8f8f2;\n \
			}\n \
			.errorDisplay {\n \
			  width: 100%;\n \
			  max-height: 100vh;\n \
			  display: flex;\n \
			  flex-direction: column;\n \
			  align-content: center;\n \
			  text-align: center;\n \
			}\n \
			img {\n \
			  margin: 0 auto;\n \
			  max-height: 100%;\n \
			}\n \
		</style>\n \
		</head>\n \
		<body>\n \
			<div class=\"errorDisplay\">\n \
				<h1>Erreur " << code << "</h1>\n \
				<p>" << mess << "</p>\n \
				<img src=\"https://http.cat/" << code << "\" alt=\"img http cat error " << code << "\" />\n \
			</div>\n \
			</body>\n \
		</html>\n";

	return (ss.str());
}

/**
 * @brief Build the http response when a error page is needed
 *
 * @param srv The Server class
 * @return A string with the http response to send
 */
const std::string ResponseError::createResponse(const Server &srv)
{
	std::string resp;
	std::string path;
	std::stringstream ss;
	std::ifstream file;
	Request dataError;
	std::string fileStr;

	dataError = this->_createDataError();
	ss << this->_code;
	path = srv.getRoot() + "/error/" + ss.str() + ".html";
	file.open(path.c_str());
	if (!file.is_open())
	{
		fileStr = createBodyHTML(this->_code, this->_message);
	}
	else {
		fileStr = createFileStr(file);
	}


	if (!addContentType(&resp, "text/html", path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path, fileStr)) 
		throw ResponseError(500, "can't add content length", dataError);
	addStartLine(&resp, this->_protocol, this->_code, this->_message);
	resp.append("\r\n\r\n");
	addBody(&resp, fileStr);
	return (resp);
}

