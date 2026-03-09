
#include "../../includes/includes.hpp"
#include "../../includes/requests/MimeTypes.hpp"
#include "../../includes/requests/Request.hpp"
#include "../../includes/requests/ResponseError.hpp"
#include <exception>

// INFO: All prototypes are in methodClass.hpp

// FOR findTarget

/**
 * @brief Find the best matching location for a given path
 *
 * This function finds the most specific location that matches the given path
 * using hierarchical matching (prefix matching with '/' boundary check).
 *
 * @param path The path to match (e.g., "/uploads/test/file.txt")
 * @param locations Vector of all configured locations
 * @return Index of the best matching location, or -1 if no match found
 */
int findBestMatchingLocation(const std::string &path, const std::vector<Location> &locations)
{
	std::string normalizedPath = path;

	if (normalizedPath.size() > 1 && normalizedPath[normalizedPath.size() - 1] == '/')
		normalizedPath = normalizedPath.substr(0, normalizedPath.size() - 1);

	int bestMatch = -1;
	size_t bestMatchLen = 0;

	for (size_t i = 0; i < locations.size(); i++)
	{
		const std::string &locPath = locations[i].getPath();
		size_t locLen = locPath.size();

		if (normalizedPath.compare(0, locLen, locPath) == 0)
		{
			if (normalizedPath.size() == locLen || normalizedPath[locLen] == '/')
			{
				if (locLen > bestMatchLen)
				{
					bestMatch = i;
					bestMatchLen = locLen;
				}
			}
		}
	}

	return bestMatch;
}

/**
 * @brief extract the wihitout the file (if terminated by a file)
 *
 * @param locPath the raw path
 * @param root the root path
 * @param dataError the struct Request for send a Response Error if needed
 * @return the path without a file if needed
 */
static std::string extractGoodPath(std::string locPath, std::string root)
{
	std::string lastPart;
	unsigned int lastSlash;

	lastSlash = locPath.find_last_of('/');
	lastPart = locPath.substr(lastSlash, locPath.size());
	if (isDir(root + locPath))
		return (locPath);
	return (locPath.substr(0, lastSlash));
}

/**
 * @brief CHeck if the path in argument is a directory
 *
 * @param path the path to check
 * @param dataError the struct Request for send a Response Error if needed
 * @return true if is a directory or false if is not
 */
bool isDir(const std::string &path)
{
	struct stat structStat;

	if (path.empty())
		return (false);
	structStat.st_mode = 0;
	stat(path.c_str(), &structStat);
	if (S_ISDIR(structStat.st_mode))
		return (true);
	return (false);
}

/**
 * @brief research et check if the path is a knowed location define in the config file and if these properties are good
 *
 * @param locPath The path to check
 * @param loc a vector of the class Location with every location define in the config file
 * @param dataError the struct Request for send a Response Error if needed
 * @param method The method requested
 * @param root the path of the root file
 * @return the full path to the target
 */
std::string findTarget(std::string locPath, std::vector<Location> loc, Request dataError, std::string method, std::string root)
{
	std::string goodPath;

	if (locPath.empty() || locPath == "/")
		goodPath = "/";
	else
		goodPath = extractGoodPath(locPath, root);
	if (goodPath.empty())
		goodPath = locPath;

	int bestMatch = findBestMatchingLocation(goodPath, loc);

	if (bestMatch == -1)
	{
		return (locPath);
	}

	if (!loc.at(bestMatch).getMethodValue(method))
	{
		if (loc.at(bestMatch).getReturn().first.empty())
			throw ResponseError(405, "Method not allowed", dataError);
		return (loc.at(bestMatch).getReturn().first);
	}

	if (loc.at(bestMatch).getAutoIndex())
		return locPath;
	else if (!loc.at(bestMatch).getIndex().empty())
	{
		if (goodPath == "/")
			return ("/" + loc.at(bestMatch).getIndex());
		else if (isDir(locPath))
			return (goodPath + "/" + loc.at(bestMatch).getIndex());
	} else if (!loc.at(bestMatch).getReturn().first.empty())
		return (loc.at(bestMatch).getReturn().first);
	return (locPath);
}

// *** ADDING LINE TO RESPONSE

/**
 * @brief add the first line of the response (ex: HTTP/1.1 200 OK)
 *
 * @param resp a string with the response already builded
 * @param protocol the protocl (1.0 or 1.1)
 * @param code the status code
 * @param mess the message associate at the status code
 * @return false if a error occured, otherwise true
 */
bool addStartLine(std::string *resp, std::string protocol, unsigned int code, std::string mess)
{
	std::stringstream ss;

	ss << code;
	try
	{
		resp->insert(0, protocol + " " + ss.str() + " " + mess + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}

	return (true);
}

/**
 * @brief extract the extension of the filename and search in the class MimTypes the corresponding type
 *
 * @param file  the full filename
 * @return the correct mimeType in relation to the extension
 */
std::string findMimeType(std::string file)
{
	std::string extension;

	extension = file.substr(file.find_last_of(".") + 1, file.length());
	return MimeTypes::getType(extension);
}

/**
 * @brief adding the content type line to the response
 *
 * @param resp the string containing the response
 * @param accept the accepted content type
 * @param file the name of the file
 * @return true if all works like excepted, otherwise false
 */
bool addContentType(std::string *resp, std::string accept, std::string file)
{
	std::string contentType;
	std::stringstream acceptSs(accept);
	std::string type;

	if (file.find("http") < file.size())
	{
		resp->append("Content-Type: text/html\n");
		return (true);
	}
	contentType = findMimeType(file);
	while (std::getline(acceptSs, type, ','))
	{
		// std::cout << "type: " << type << ", Content-Type: " << contentType << std::endl;
		if (contentType == type || contentType.compare(0, 3, "*/*"))
		{
			resp->append("Content-Type: " + contentType + "\n");
			return (true);
		}
	}
	return (false);
}

/**
 * @brief Adding the contentType line with a specific type
 *
 * @param resp A string containing the response
 * @param type the specific type to add
 * @return true if works like excepted, otherwise false
 */
bool addContentType(std::string *resp, std::string type)
{
	try
	{
		resp->append("Content-Type: " + type + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

/**
 * @brief adding the date line to the response
 *
 * @param resp containing the response
 * @return true if everything works lke excepted, otherwise false
 */
bool addDate(std::string *resp)
{
	char buff[100];
	tm *time;
	time_t tt;

	std::time(&tt);
	time = std::localtime(&tt);
	if (!time)
		return (false);
	std::strftime(buff, 100, "Date: %a, %d %b %Y %X GMT\n", time);
	resp->append(buff);
	return (true);
}

/**
 * @brief adding the last modifed line to the response
 *
 * @param resp a string containing the response
 * @param pathTarget the path to the traget file
 * @return true if everthing works as excepted, otherwise false
 */
bool addLastModif(std::string *resp, std::string pathTarget)
{
	struct stat buff;
	char date[100];
	tm *time;
	time_t tt;
	std::ifstream file(pathTarget.c_str());
	std::string dateString;

	if (!file.is_open())
	{
		std::time(&tt);
		time = std::localtime(&tt);
		std::strftime(date, 100, "Last-Modified: %a, %d %b %Y %X GMT\n", time);
		resp->append(date);

		return (true);
	}
	stat(pathTarget.c_str(), &buff);
	tt = buff.st_mtim.tv_sec;
	try
	{
		time = std::localtime(&tt);
		std::strftime(date, 100, "%a, %d %b %Y %X GMT\n", time);
		dateString = date;
		resp->append("Last-Modified: " + dateString);
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

/**
 * @brief adding the content lenght line
 *
 * @param resp a string with the response
 * @param path the path the file
 * @return true if everything works as excepted, otherwise false
 */
bool addContentLenght(std::string *resp, std::string path)
{
	unsigned int size;
	std::stringstream ss;
	struct stat buff;

	if (path.find("http") < path.size())
	{
		resp->append("Content-Length: 0\n");
		return true;
	}
	stat(path.c_str(), &buff);
	size = buff.st_size;
	ss << size;
	try
	{
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

/**
 * @brief adding the content lenght file, with the contebt of the file in argument
 *
 * @param resp a string containing the response
 * @param path a string containing the path to the file
 * @param fileStr a string containing the content of the file
 * @return true if everything works as excepted, otherwise false
 */
bool addContentLenght(std::string *resp, std::string path, std::string fileStr)
{
	unsigned int size;
	std::stringstream ss;
	struct stat buff;
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		size = fileStr.size();
		ss << size;
		resp->append("Content-Length: " + ss.str() + "\n");
		return (true);
	}
	stat(path.c_str(), &buff);
	size = buff.st_size;
	ss << size;
	try
	{
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

/**
 * @brief adding the content lenght line with a predefined size
 *
 * @param resp a string with the response
 * @param bodySize The size of the content
 * @return true if everything works as excepted, otherwise false
 */
bool addContentLenght(std::string *resp, ssize_t bodySize)
{
	std::stringstream ss;

	ss << bodySize;
	try
	{
		resp->append("Content-Length: " + ss.str() + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}

/**
 * @brief adding the body to the response
 *
 * @param resp a string containing the response
 * @param file The content to add to the response
 * @return return true if everything works as excepted, otherwise false
 */
bool addBody(std::string *resp, std::string file)
{
	resp->append("\n" + file + "\r\n\r\n");
	return (true);
}

/**
 * @brief adding the location line to the response
 *
 * @param resp a string containing the response
 * @param location the location to add to the response
 * @return true if everything works as excepted, otherwise false
 */
bool addLocation(std::string *resp, std::string location)
{
	try
	{
		resp->append("Location: " + location + "\n");
	} catch (std::exception &e)
	{
		return (false);
	}
	return (true);
}
