#include "../../includes/requests/Request.hpp"

static void setHeader(std::string line, Request& req)
{
	std::stringstream ss(line);
	std::string word;

	ss >> word;
	if (word != "GET" &&
		word != "POST" &&
		word != "DELETE")
	{
		// do something with error
	}
	req._method = word;

	ss >> word;
	req._location = word;

	ss >> word;
	if (word != "HTTP/1.1" &&
		word != "HTTP/1.0")
	{
		// do something with error
	}
	req._protocol = word;
}

static std::map<std::string, std::string*> createMap(Request &req)
{
	std::map<std::string, std::string*> ret;

	ret.insert(std::make_pair(std::string("Host:"), &req._host));
	ret.insert(std::make_pair(std::string("User-Agent:"), &req._userAgent));
	ret.insert(std::make_pair(std::string("Accept:"), &req._accept));
	ret.insert(std::make_pair(std::string("Content-Type:"), &req._ContentType));

	return ret;
}

Request createRequest(char* buffer)
{
	Request ret;
	std::map<std::string, std::string*> ptrMap = createMap(ret);
	std::istringstream iss(buffer);
	std::string line;


	iss >> line;
	setHeader(line, ret);
	while (std::getline(iss, line))
	{
		std::stringstream ss(line);
		std::string word;
		ss >> word;
		if (word == "Content-Length:")
		{
			unsigned int cLength;
			ss >> cLength;
			ret._ContentLength = cLength;
			continue ;
		}
		try
		{
			std::string* strPtr = ptrMap.at(word);
			ss >> word;
			*strPtr = word;
		}
		catch(...)
		{
			// do something with error
		}
	}

	std::string body;
	std::getline(iss, body, '\0');
	ret._body = body;

	return ret;
}

// static Get* createGet(char* buffer)
// {
	
// }

// static Post* createPost(char* buffer)
// {
	
// }

// static Delete* createDelete(char* buffer)
// {

// }

// static std::string findMethod(std::string buffer)
// {
// 	std::stringstream ss(buffer);
// 	std::string method;

// 	ss >> method;
// 	return method;
// }

// static Methods* createMethod(char* buffer)
// {
// 	Methods* ret;
// 	std::string method;

// 	method = findMethod(std::string(buffer));

// 	if (method == "GET")
// 		ret = createGet(buffer);
// 	else if (method == "POST")
// 		ret = createPost(buffer);
// 	else if (method == "DELETE")
// 		ret = createDelete(buffer);
// 	// else
// 	//	throw wrong method error
	
// 	return ret;
// }
