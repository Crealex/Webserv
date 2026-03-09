#ifndef METHODS_CLASS_HPP

#define METHODS_CLASS_HPP
#include "../Server.hpp"
#include "../includes.hpp"

class Request;

class Methods
{
  protected:
	std::string _method;
	std::string _location;
	std::string _protocol;
	std::string _host;
	Request _createDataError();
	Methods(Request requ);

  public:
	/**
	 * @brief a virtual funtction for build the reponse, implementation in get.cpp, post.cpp, delete.cpp, ResponseError.cpp
	 */
	virtual const std::string createResponse(const Server &srv) = 0;
	virtual ~Methods();
};

// methodsUtils.cpp, all prototypes for build the response in delete.cpp, errorResponse.cpp, get.cpp and post.cpp

bool addStartLine(std::string *resp, std::string protocol, unsigned int code, std::string mess);
bool addContentType(std::string *resp, std::string accept, std::string file);
bool addDate(std::string *resp);
bool addLastModif(std::string *resp, std::string pathTarget);
bool addContentLenght(std::string *resp, std::string path);
bool addContentLenght(std::string *resp, ssize_t bodySize);
bool addContentLenght(std::string *resp, std::string path, std::string fileStr);
bool addBody(std::string *resp, std::string file);
bool addLocation(std::string *resp, std::string host, std::string location);
bool addLocation(std::string *resp, std::string location);
bool addContentType(std::string *resp, std::string type);
int findBestMatchingLocation(const std::string &path, const std::vector<Location> &locations);
std::string findTarget(std::string locPath, std::vector<Location> loc, Request dataError, std::string method, std::string root);
bool isDir(const std::string &path);

Methods *createMethod(std::string buffer, size_t maxSize, bool &ret);
Request createRequest(std::string buffer, size_t maxSize);

// autoIndex.cpp
std::string createHTMLAutoIndex(const std::string &path, const std::string &location);

#endif
