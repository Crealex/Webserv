#include "includes.hpp"
#include "configStruct.hpp"

#define pairString std::string, std::string

class Location {

	private:

		bool						_autoIndex;
		std::string					_index;
		std::string					_ret;
		std::string					_uploadPath;
		std::string					_path;
		std::map<std::string, bool>	_allowedMethods;
		std::map<pairString>		_cgiHandler;

	public:

		Location(location src, std::string root);
		Location(const Location& cpy);
		Location& operator=(const Location& src);
		~Location();

		bool getAutoIndex() const;
		std::string getIndex() const;
		std::string getReturn() const;
		std::string getUploadPath() const;
		std::string getPath() const;
		bool getMethodValue(std::string key) const;
		std::map<std::string, bool> getAllowedMethods() const;
		std::map<pairString> getCgiHandler() const;

		void print() const;
};

std::vector<Location> createLocations(server serv);
std::ostream& operator<<(std::ostream &os, std::map<pairString> map);
std::ostream& operator<<(std::ostream &os, std::map<std::string, bool> map);