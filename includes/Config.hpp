#ifndef CONFIG_HPP

#define CONFIG_HPP

#include "includes.hpp"

typedef std::pair<std::string, unsigned int> addPort_t;

struct serverData
{
	std::string	name;
	std::vector<addPort_t>	addressPort;
};

struct errorData
{
	std::vector<unsigned int>	code;
	std::string 				path;
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
	std::vector<serverData>		addressPort;	// port (maybe can be a array)
	std::vector<errorData>		errorPage;		// all the error codes
	unsigned int				maxSize;		// max size bodies request
	std::vector<std::string>	sitesName;
	std::map<std::string, site>	sites;			// key = site name, value = struct of element

	void 	siteParsing(struct siteParse site);
	void	parseElt(struct structParse);

  public:
	Config(std::string pathConfig);
	~Config();

	
	// GETTER
	std::vector<serverData>		getAddressPort() const;
	std::vector<errorData>		getErrorPage() const;
	unsigned int				getMaxSize() const;
	std::vector<std::string>	getSitesName() const;

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
