#ifndef MIME_TYPES_HPP

# define MIME_TYPES_HPP
# include <map>
# include <string>

/**
 * @class MimeTypes
 * @brief static class with a map representive all mimes tupe handle by our server
 *
 */
class MimeTypes
{
	private:
		static const std::map<std::string, std::string> _mimeTypes;
		static std::map<std::string, std::string> initMap();
	public:
		static const std::string getType(std::string extension);
};


#endif
