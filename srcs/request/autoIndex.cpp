#include "../../includes/requests/Method.hpp"
#include "../../includes/requests/fileInfo.hpp"
#include <cstddef>
#include <dirent.h>
#include <sstream>
#include <vector>

std::vector<fileInfo> fillFileInfos(const std::string &path)
{
	std::vector<fileInfo> filesInfos;
	fileInfo current;
	DIR *dir;
	struct dirent *structDir = new dirent;

	dir = opendir(path.c_str());
	structDir = readdir(dir);
	while (structDir)
	{
		current.name = structDir->d_name;
		filesInfos.push_back(current);
		structDir = readdir(dir);
	}
	return (filesInfos);
}

std::string listContents(std::vector<fileInfo> files)
{
	std::stringstream ret;

	ret << "<ul>\n";
	for (size_t i = 0; i < files.size(); i++)
	{
		ret << "<li>" << files.at(i).name << "</li>\n";
	}
	ret << "</ul>\n";
	return (ret.str());
}

std::string createHTMLAutoIndex(const std::string &path)
{
	std::stringstream body;
	std::vector<fileInfo> filesInfos;

	filesInfos = fillFileInfos(path);
	body << "<!doctype html>\n<html lang=\"en\">\n \
			<head>\n \
			<link href=\"./style.css\" rel=\"stylesheet\"/>\n \
			<link rel=\"icon\" type=\"image/png\" href=\"./LogoDAK.png\"</head>\
			<body>\n \
			<main>\n \
			<div class=\"intro\">\n \
			<h2>" << path << " is a dir </h2>\n \
			<h3>Files list:</h3>\n \
			" << listContents(filesInfos) << "\
			</div>\n \
			</main>\n \
			</body>\n \
			</html>\n";
	return (body.str());
}
