
#include <string>

struct fileInfo {
	std::string	name;
	bool		isDir;
	off_t		size;
	time_t		lastModified;
};
