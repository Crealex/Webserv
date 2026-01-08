#ifndef METHODS_CLASS_HPP

#define METHODS_CLASS_HPP
#include "../includes.hpp"
#include "../Config.hpp"

struct Request;

class Methods
{
  protected:
	std::string _method;
	std::string _location;
	std::string _protocol;
	std::string _host;
	Methods(Request requ);

  public:
	/**
	 * @brief a virtual funtction for build the reponse, implementation in get.cpp, post.cpp, delete.cpp, ResponseError.cpp
	 */
	virtual const std::string createResponse(Config conf) = 0;
	virtual ~Methods();
};

// addLineToResp.cpp, all prototypes for build the response in delete.cpp, errorResponse.cpp, get.cpp and post.cpp

bool	addStartLine(std::string *resp, std::string protocol, unsigned int code, std::string mess);
bool	addContentType(std::string *resp, std::string accept, std::string file);
bool	addDate(std::string *resp);
bool	addLastModif(std::string *resp, std::string pathTarget);
bool	addContentLenght(std::string *resp, std::string path);
bool	addBody(std::string *resp, std::string file);
bool	addLocation(std::string *resp, std::string host, std::string location);
bool	addContentType(std::string *resp, std::string type);

Methods* createMethod(char *buffer, size_t maxSize);
Request createRequest(char* buffer, size_t maxSize);

#endif
