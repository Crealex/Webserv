#include "../../includes/requests/Request.hpp"

static void setHeader(std::string line, Request& req)
{
	std::stringstream ss(line);
	std::string method;
	std::string location;
	std::string protocol;

	ss >> method;
	if (method != "GET" &&
		method != "POST" &&
		method != "DELETE")
	{
		// do something with error
	}
	req._method = method;

	ss >> location;
	if (access(location.c_str(), F_OK) != 0)
	{
		// do something with error
	}
	req._location = location;

	ss >> protocol;
	if (protocol != "HTTP/1.1" &&
		protocol != "HTTP/1.0")
	{
		// do something with error
	}
	req._protocol = protocol;
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


	std::getline(iss, line);
	setHeader(line, ret);
	while (std::getline(iss, line))
	{
		if (line.empty())
			break ;
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
	while (std::getline(iss, body))
	{
		body += '\n';
		ret._body += body;
	}
	// need to check content type
	if (ret._host.empty() || access(ret._host.c_str(), F_OK) != 0)
	{
		// do something with error
	}

	return ret;
}

// int main()
// {
// 	// test GET
// 	// std::string str = "GET /contact HTTP/1.1\nHost: exemple.fr\nUser-Agent: curl/8.6.0\nAccept: */*";

// 	// test POST 1
// 	// std::string str = "POST /test HTTP/1.1\nHost: exemple.fr\nContent-Type: application/x-www-form-urlencoded\nContent-Length: 27\n\nfield1=value1&field2=value2";

// 	// test POST 2 (si qqun arrive a le faire marcher ?)
// 	// std::string str = "POST /test HTTP/1.1\nHost: exemple.fr\nContent-Type: multipart/form-data;boundary="delimiter12345"\n\n--delimiter12345\nContent-Disposition: form-data; name="field1"\n\nvalue1\n--delimiter12345\nContent-Disposition: form-data; name="field2"; filename="exemple.txt"\n\nvalue2\n--delimiter12345--";

// 	// test DELETE
// 	std::string str = "DELETE /fichier.html HTTP/1.1\nHost: example.com";

// 	Request req;

// 	req = createRequest((char *)str.c_str());

// 	std::cout << "method = " << req._method
// 		<< "\nlocation = " << req._location
// 		<< "\nprotocol = " << req._protocol
// 		<< "\nhost = " << req._host
// 		<< "\nuserAgent = " << req._userAgent
// 		<< "\naccept = " << req._accept
// 		<< "\ncontent type = " << req._ContentType
// 		<< "\ncontent length = " << req._ContentLength
// 		<< "\nbody = " << req._body << std::endl;
// }

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
