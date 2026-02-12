#include "../../includes/requests/MimeTypes.hpp"
#include <exception>
#include <map>

std::map<std::string, std::string> MimeTypes::initMap()
{
	std::map<std::string, std::string> m;
	m["html"] = "text/html";
	m["css"] = "text/css";
	m["png"] = "image/png";
	m["jpg"] = "image/png";
	m["mp4"] = "video/mp4";
	m["md"] = "text/markdown";
	m["json"] = "application/json";
	m["js"] = "text/javascript";
	m["mp3"] = "audio/mpeg";
	//m["jpeg"] = "image/jpeg";
	//m["jpg"] = "image/jpeg";
	m["csv"] = "text/csv";
	m["gif"] = "image/gif";
	m["wav"] = "audio/wav";
	m["zip"] = "application/zip";
	m["pdf"] = "application/pdf";
	m["ico"] = "image/vnd.microsoft.icon";
	m["bin"] = "application/octet-stream";
	m["avi"] = "video/x-msvideo";
	m["avif"] = "image/avif";
	m["txt"] = "text/plain";
	m[""] = "multipart/form-data";
	return (m);
}

const std::map<std::string, std::string> MimeTypes::_mimeTypes = initMap();

const std::string MimeTypes::getType(std::string extension)
{
	try
	{
		return MimeTypes::_mimeTypes.at(extension);
	}
	catch (std::exception &e)
	{
		return ("application/octet-stream");
	}
}
