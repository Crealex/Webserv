#include "../../includes/requests/Get.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/requests/ResponseError.hpp"

Get::Get(const Request &requ): Methods(requ), _userAgent(requ.getUserAgent()), _accept(requ.getAccept()) 
{
    // std::cout << GREEN << "Default Get constructor called" << RESET << std::endl;
}


/**
 * @brief Create un string with de content of the file
 *
 * @param file 
 * @return 
 */
static std::string createFileStr(std::ifstream &file)
{
	std::string fileStr;
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	fileStr.resize(size);
	file.read(&fileStr[0], size);

	return fileStr;
}

/**
 * @brief Identify the status and the message associted
 *
 * @param srv The class Server
 * @return A pair with the a unisgned int (the code) and a std::string (the message)
 */
std::pair<unsigned int, std::string> Get::_findCodeMess(const Server &srv)
{
	std::pair<unsigned int, std::string> ret;
	int finded = -1;

	for (size_t i = 0; i < srv.getLocations().size(); i++)
	{
		if (this->_location == srv.getLocations().at(i).getPath())
		{
			finded = i;
			break ;
		}
	}
	if (finded == -1)
		return (std::pair<unsigned int, std::string>(200, "OK"));
	if (srv.getLocations().at(finded).getReturn().first.empty())
		return (std::pair<unsigned int, std::string>(200, "OK"));
	ret.first = srv.getLocations().at(finded).getReturn().second;
	switch (ret.first) 
	{
		case 301:	ret.second = "Moved Permanently";
					break;
		case 307:	ret.second = "Temporary Redirect";
					break;
		case 308:	ret.second = "Permanent Redirect";
					break;
		default:	ret.second = "undefinded";
					break;
	}
	return (ret);
}

/**
 * @brief building the response for get request
 *
 * @param srv The server class
 * @return A string with de response to send
 */
const std::string Get::createResponse(const Server &srv)
{
	std::string		resp;
	std::ifstream	file;
	std::string		path;
	Request			dataError;
	std::string		target;
	std::string fileStr;
	std::pair<unsigned int, std::string> codeMess;
	bool isRedir = 0;
	
	dataError = this->_createDataError();
	dataError.setAccept(this->_accept);
	dataError.setUserAgent(this->_userAgent);

	target = findTarget(this->_location, srv.getLocations(), dataError, "GET");
	if (target.find("http") == std::string::npos)
	{
		path = srv.getRoot() + "/" + target; // TODO: Peut-etre retirer le /
		file.open(path.c_str());
		if (!file.is_open())
			throw ResponseError(404, "Not found", dataError);
		fileStr = createFileStr(file);
	}
	else
	{
		isRedir = 1;
		path = target;
	}

	codeMess = _findCodeMess(srv);
	if (isRedir)
	{
		addLocation(&resp, path);
		addContentType(&resp, this->_accept, path);
		addDate(&resp);
		addContentLenght(&resp, 0); // Content-Length = 0
		addStartLine(&resp, this->_protocol, codeMess.first, codeMess.second);
		resp.append("\n\r\n\r");
		return (resp);
	}
	if (!addContentType(&resp, this->_accept, path))
		throw ResponseError(406, "Not acceptable", dataError);
	if (!addDate(&resp))
		throw ResponseError(500, "Can't add date", dataError);
	if (!addLastModif(&resp, path))
		throw ResponseError(500, "can't add last modif", dataError);
	if (!addContentLenght(&resp, path))
		throw ResponseError(500, "can't add content length", dataError);
	if (!addBody(&resp, fileStr))
		throw ResponseError(500, "can't add body", dataError);
	if (!addStartLine(&resp, this->_protocol, codeMess.first, codeMess.second))
		throw ResponseError(500, "can't add start line", dataError);

	return (resp);
}

