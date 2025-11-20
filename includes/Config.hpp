#ifndef CONFIG_HPP

#define CONFIG_HPP

#include <iostream>
#include <map>
#include <vector>

struct pair
{
	std::string		p1;
	unsigned int	p2;
};

struct site
{
	std::map<std::string, bool>	method;	 		// List of accepted methods for the route (Not sure of name and variabel type)
	std::vector<std::string>	redirection;	// HTTP redirection
	std::string					dirRoot;		// not sure
	bool						dirListing;		// Enable or disable listening of directory
	std::string					defaultFile;	// default file when the request ressource is a directory
	bool						uploadFiles;	// Enable or disable uploading files from the clients to the server is authorized, and storage location is provided
	std::string					CGI;			// path of the CGI (maybe need more rule)
};

class Config
{
  private:
	std::vector<std::string>			hostname;
	std::map<std::string, pair>			addressPort;// port (maybe can be a array)
	std::vector<unsigned int>			errorCode;	// all the error codes
	std::string							errorPath;	// Path of default error page (maybe needed error code)
	unsigned int						maxSize;	// max size bodies request
	std::map<std::string, site>			sites;		// key = site name, value = struct of element

  public:
	Config(structParse data);
	~Config();

	// GETTER FROM addressPort
	std::vector<pair>	getAddressPort() const;

	// GETTER
	std::vector<unsigned int>	getErrorCode() const;
	std::string					getErrorPath() const;
	unsigned int				getMaxSize() const;

	// GETTER FROM struct site
	std::map<std::string, bool>	getMethod(std::string siteName) const;
	std::vector<std::string>	getRedirection(std::string siteName) const;
	std::string					getDirRoot(std::string siteName) const;
	bool						getDirListing(std::string siteName) const;
	std::string					getDefaultFile(std::string siteName) const;
	bool						getUploadFiles(std::string siteName) const;
	std::string					getCGI(std::string siteName) const;
};

#endif
