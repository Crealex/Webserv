#include "includes.hpp"
#include "configStruct.hpp"

#define pairString std::pair<std::string, std::string>

class Location {

	private:

		bool						_autoIndex;
		std::string					_index;
		std::string					_ret;
		std::string					_uploadPath;
		std::string					_path;
		std::vector<std::string>	_allowedMethods;
		std::vector<pairString>		_cgiHandler;

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
		std::vector<std::string> getAllowedMethods() const;
		std::vector<pairString> getCgiHandler() const;
};

std::vector<Location> createLocations(server serv);
