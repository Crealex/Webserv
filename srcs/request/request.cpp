#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"

/**
 * @brief return a hard vector with the different accepted Content-Type
 * 
 * @return Vector with the Content-Type
 */
static std::vector<std::string> acceptedType()
{
	std::vector<std::string> v;

	v.push_back("*/*");
	v.push_back("text/html");
	v.push_back("text/css");
	v.push_back("image/png");
	v.push_back("video/mp4");
	v.push_back("text/markdown");
	v.push_back("application/json");
	v.push_back("text/javascript");
	v.push_back("audio/mpeg");
	v.push_back("image/jpeg");
	v.push_back("text/csv");
	v.push_back("image/gif");
	v.push_back("audio/wav");
	v.push_back("application/zip");
	v.push_back("application/pdf");
	v.push_back("image/vnd.microsoft.icon");
	v.push_back("application/octet-stream");
	v.push_back("video/x-msvideo");
	v.push_back("image/avif");
	v.push_back("text/plain"); // INFO: ajouter par alex
	v.push_back("multipart/form-data"); // INFO: ajouté par alex

	return v;
}

static void checkGET(Request req)
{
	std::cout << "in checkGET" << std::endl;
	if (req._userAgent.empty() || 
		req._accept.empty())
	{
		throw ResponseError(411, "Error: Missing value", req);
	}

	int leave = 0;
	std::vector<std::string> v = acceptedType();
	for (std::vector<std::string>::iterator it = v.begin();
	it != v.end(); it++)
	{
		std::istringstream iss(req._accept);
		std::string str;
		while (std::getline(iss, str, ','))
		{
		std::cout << "*it: " << *it << ", str: " << str << std::endl;
			if (str.compare(*it) == 0)
			{
				std::cout << "is egal" << std::endl;
				leave = 1;
				break ;
			}
		}

		if (leave == 1)
			break ;

		if (it + 1 == v.end())
			throw ResponseError(415, "Error: Unsupported Media Type", req);
	}
}

static void checkPost(Request req)
{
	int leave = 0;
	std::cout << "in checkPost" << std::endl;
	if (req._ContentType.empty() || 
		req._body.empty())
	{
		throw ResponseError(411, "Error: Missing value", req);
	}

	std::vector<std::string> v = acceptedType();
	for (std::vector<std::string>::iterator it = v.begin(); // INFO: Toutes la boucle for complémentent modif (par alex avec l'aval de kiki)
	it != v.end(); it++)
	{
		std::istringstream iss(req._accept);
		std::string str;
		while (std::getline(iss, str, ','))
		{
		std::cout << "*it: " << *it << ", str: " << str << std::endl;
			if (str.compare(*it) == 0)
			{
				std::cout << "is egal" << std::endl;
				leave = 1;
				break ;
			}
		}

		if (leave == 1)
			break ;

		if (it + 1 == v.end())
			throw ResponseError(415, "Error: Unsupported Media Type", req);
	}
	// TODO check if content length < maxSize
}

/**
 * @brief Parse the request header : The method, the location and the protocol
 * 
 * @param line 
 * @param req 
 */
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
		throw ResponseError(501, "Error: Not implemented method", req);
	}
	req._method = method;

	ss >> location;
	req._location = location;
	
	ss >> protocol;
	if (protocol != "HTTP/1.1" &&
		protocol != "HTTP/1.0")
	{
		throw ResponseError(505, "Error: HTTP version not supported", req);
	}

	req._protocol = protocol;
}

/**
 * @brief Create a map with key and pointer to the right string in the Request object
 * 
 * @param req 
 * @return std::map<std::string, std::string*> 
 */
static std::map<std::string, std::string*> createMap(Request &req)
{
	std::map<std::string, std::string*> ret;

	ret.insert(std::make_pair(std::string("Host:"), &req._host));
	ret.insert(std::make_pair(std::string("User-Agent:"), &req._userAgent));
	ret.insert(std::make_pair(std::string("Accept:"), &req._accept));
	ret.insert(std::make_pair(std::string("Content-Type:"), &req._ContentType));
	
	return ret;
}

/**
 * @brief Parse the client request and create a Request structure
 * 
 * @param buffer the client request
 * @return The created Request object
 */
Request createRequest(char* buffer)
{
	Request ret;
	std::map<std::string, std::string*> ptrMap = createMap(ret);
	std::cout << "Test requ" << std::endl;
	std::istringstream iss(buffer);
	std::string line;

	std::getline(iss, line);
	setHeader(line, ret);
	while (std::getline(iss, line)) 
	{
		if (!line.empty() && line[line.size() - 1] == '\r') // INFO: Ajouter par Alex (pour gerer les \r)
        line.erase(line.size() - 1);
		// extract and parse the different element of the request
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
			std::string str;
			while (ss >> word)
				str.append(word + ' ');
			str.erase(str.end() - 1);
			*strPtr = str;
		}
		catch(...)
		{
		}
	}

	// extract the body of the request
	std::string body;
	while (std::getline(iss, body))
	{
		body += '\n';
		ret._body += body;
	}
	
	// verify the different extracted element
	// if (ret._host.empty() || access(ret._host.c_str(), F_OK) != 0)
	// {
	// 	throw ResponseError(400, "Error: Host cannot be accessed", ret);
	// }

	if (ret._method == "POST")
		checkPost(ret);
	else if (ret._method == "GET")
		checkGET(ret);

	std::cout << std::endl << std::endl
		<< "method = " << ret._method
		<< "\nlocation = " << ret._location
		<< "\nprotocol = " << ret._protocol
		<< "\nhost = " << ret._host
		<< "\nuserAgent = " << ret._userAgent
		<< "\naccept = " << ret._accept
		<< "\ncontent type = " << ret._ContentType
		<< "\ncontent length = " << ret._ContentLength
		<< "\nbody = \n" << ret._body << std::endl;

	return ret;
}

// int main()
// {
	// test GET
	// std::string str = "GET /contact HTTP/1.1\nHost: exemple.fr\nUser-Agent: curl/8.6.0\nAccept: */*";

	// test POST 1
	// char str[] = "POST /test HTTP/1.1\nHost: exemple.fr\nContent-Type: application/x-www-form-urlencoded\nContent-Length: 27\n\nfield1=value1&field2=value2";

	// test POST 2 (si qqun arrive a le faire marcher ?)
	// char str[] = "POST /test HTTP/1.1\nHost: ./.\nContent-Type: text/html\nContent-Length: 50\n\n--delimiter12345\nContent-Disposition: form-data; name=\"field1\"\n\nvalue1\n--delimiter12345\nContent-Disposition: form-data; name=\"field2\"; filename=\"exemple.txt\"\n\nvalue2\n--delimiter12345--";

	// test DELETE
	// char str[] = "DELETE /fichier.html HTTP/1.1\nHost: example.com";

	// std::cout << "test :\n" << str << std::endl;

// 	Request req = createRequest(str);
	
// 	std::cout 
// 		<< "method = " << req._method
// 		<< "\nlocation = " << req._location
// 		<< "\nprotocol = " << req._protocol
// 		<< "\nhost = " << req._host
// 		<< "\nuserAgent = " << req._userAgent
// 		<< "\naccept = " << req._accept
// 		<< "\ncontent type = " << req._ContentType
// 		<< "\ncontent length = " << req._ContentLength
// 		<< "\nbody = \n" << req._body << std::endl;
// }
