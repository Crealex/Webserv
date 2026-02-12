#ifndef CONFIG_STRUCT_HPP

#define CONFIG_STRUCT_HPP
#include "includes.hpp"

struct location {
	std::string autoIndex;
	std::string allowedMethods;
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

typedef std::pair<std::string, unsigned int> addPort_t;

std::vector<server> createVectStructSrv(std::string configPath);

#endif // !CONFIG_STRUCT_HPP
