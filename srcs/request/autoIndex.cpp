#include "../../includes/requests/Method.hpp"
#include "../../includes/requests/fileInfo.hpp"
#include <cstddef>
#include <ctime>
#include <dirent.h>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>

//TODO: Faire la gestion des erreurs pour toutes les fonctions ci-dessus

std::vector<fileInfo> fillFileInfos(const std::string &path)
{
	std::vector<fileInfo> filesInfos;
	fileInfo current;
	DIR *dir;
	std::string fullPath;
	struct dirent *structDir;
	struct stat fileStat;

	dir = opendir(path.c_str());
	if (!dir)
		throw std::runtime_error("Error wuth opendir");
	structDir = readdir(dir);
	while (structDir)
	{
		current.name = structDir->d_name;
		fullPath = path + "/" + current.name;
		if (stat(fullPath.c_str(), &fileStat) == 0)
		{
			current.size = fileStat.st_size;
			current.lastModified = fileStat.st_mtim.tv_sec;
			current.isDir = S_ISDIR(fileStat.st_mode);
		}
		else 
		{
			current.size = 0;
			current.lastModified = 0;
			current.isDir = false;
		}
		filesInfos.push_back(current);
		structDir = readdir(dir);
	}
	closedir(dir);
	return (filesInfos);
}

static std::string buildPath(const std::string& location, const std::string& name)
{
	std::string ret;

	ret = location + "/" + name;
	if (name == ".")
		ret = location;
	else if (name == "..")
	{
		size_t lastSlash = location.rfind('/');
		if (lastSlash != std::string::npos && lastSlash > 0)
			ret = location.substr(0, lastSlash);
		else
			ret = "/";
	}
	return (ret);
}

std::string listContents(std::vector<fileInfo> files, const std::string &path)
{
	std::stringstream ret;
	char date[256];
	tm *time;


	ret << "<table>\n \
			<tr>\n<th>Name</th>\n<th>Size</th>\n<th>Last Modified</th>\n";
	for (size_t i = 0; i < files.size(); i++)
	{
		time = std::localtime(&files.at(i).lastModified);
		std::strftime(date, 100, "%d %b %Y %X GMT", time);
		ret << "<tr><td><a href=\"" << buildPath(path, files.at(i).name) << "\">" << files.at(i).name << (files.at(i).isDir ? "/ " :  " ") << "</a></td><td>"<< files.at(i).size * 0.00125 << "kb</td><td>" << date << "</td></tr>\n";
	}
	ret << "</table>\n";
	return (ret.str());
}

std::string createHTMLAutoIndex(const std::string &path, const std::string &location)
{
	std::stringstream body;
	std::vector<fileInfo> filesInfos;

	filesInfos = fillFileInfos(path);
	body << "<!doctype html>\n<html lang=\"en\">\n \
			<head>\n \
			<link href=\"/style.css\" rel=\"stylesheet\"/>\n \
			<link rel=\"icon\" type=\"image/png\" href=\"/LogoDAK.png\"</head>\
			<body>\n \
			<main>\n \
			<div class=\"intro\">\n \
			<h2>Content list of " << location << "</h2>\n \
			" << listContents(filesInfos, location) << "\
			</div>\n \
			</main>\n \
			</body>\n \
			</html>\n";
	return (body.str());
}
