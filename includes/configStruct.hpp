#ifndef CONFIG_STRUCT_HPP

#define CONFIG_STRUCT_HPP
#include <string>
#include <vector>

struct location {
	std::string autoIndex;
	std::vector<std::string> allowedMethods;
	std::string index;
	std::string ret;
	std::vector<std::string> cgi;
	std::string uploadPath;
	std::string path;
};

struct server {
	std::string hostname;
	std::vector<std::string> listen;
	std::string root;
	std::string maxSize;
	std::vector<std::string> errorPages;
	std::vector<location> locations;
};

#endif // !CONFIG_STRUCT_HPP
