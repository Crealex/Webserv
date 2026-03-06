#include "../../includes/requests/Get.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/colors.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseBuilder.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <exception>

Get::Get(const Request &requ) : Methods(requ), _userAgent(requ.getUserAgent()), _accept(requ.getAccept())
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
		try
		{
			if (this->_location == srv.getLocations().at(i).getPath())
			{
				finded = i;
				break;
			}
		} catch (const std::exception &)
		{
		}
	}
	if (finded == -1)
		return (std::pair<unsigned int, std::string>(200, "OK"));
	if (srv.getLocations().at(finded).getReturn().first.empty())
		return (std::pair<unsigned int, std::string>(200, "OK"));
	ret.first = srv.getLocations().at(finded).getReturn().second;
	switch (ret.first)
	{
	case 301:
		ret.second = "Moved Permanently";
		break;
	case 307:
		ret.second = "Temporary Redirect";
		break;
	case 308:
		ret.second = "Permanent Redirect";
		break;
	default:
		ret.second = "undefinded";
		break;
	}
	return (ret);
}

bool Get::_isAllowedAutoIndex(const Server &srv)
{
	int bestMatch = findBestMatchingLocation(this->_location, srv.getLocations());

	if (bestMatch != -1)
		return srv.getLocations().at(bestMatch).getAutoIndex();

	return (false);
}

/**
 * @brief building the response for get request
 *
 * @param srv The server class
 * @return A string with de response to send
 */
const std::string Get::createResponse(const Server &srv)
{
	std::ifstream file;
	std::string path;
	Request dataError;
	std::string target;
	std::string fileStr;
	std::pair<unsigned int, std::string> codeMess;
	bool isRedir = 0;

	dataError = this->_createDataError();
	dataError.setAccept(this->_accept);
	dataError.setUserAgent(this->_userAgent);

	ResponseBuilder resp(dataError, this->_protocol);
	target = findTarget(this->_location, srv.getLocations(), dataError, "GET", srv.getRoot());
	// std::cout << "target: " << target << std::endl;
	if (target.find("http") == std::string::npos)
	{
		path = srv.getRoot() + "/" + target;
		if (!isDir(path))
		{
			file.open(path.c_str());
			if (!file.is_open())
				throw ResponseError(404, "Not found", dataError);
			fileStr = createFileStr(file);
		}
	} else
	{
		isRedir = 1;
		path = target;
	}

	codeMess = _findCodeMess(srv);
	if (isRedir)
	{
		resp.location(path)
			.contentType(this->_accept, path)
			.date()
			.contentLength(0)
			.startLine(codeMess.first, codeMess.second)
			.append("\r\n\r\n");
	} else if (isDir(path))
	{
		if (!this->_isAllowedAutoIndex(srv))
			throw ResponseError(401, "Unauthorized", dataError);
		try
		{
			fileStr = createHTMLAutoIndex(path, this->_location);
		} catch (...)
		{
			throw ResponseError(500, "Building HTML autoIndex failed", dataError);
		}
		resp.contentType("text/html")
			.date()
			.lastModified(path)
			.contentLength(fileStr.size())
			.body(fileStr)
			.startLine(codeMess.first, codeMess.second)
			.append("\r\n\r\n");
	} else
	{
		resp.contentType(this->_accept, path)
			.date()
			.lastModified(path)
			.contentLength(path)
			.body(fileStr)
			.startLine(codeMess.first, codeMess.second)
			.append("\r\n\r\n");
	}
	return (resp.build());
}
