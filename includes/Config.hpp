#ifndef CONFIG_HPP

#define CONFIG_HPP

#include <iostream>
#include <map>

struct site
{
	std::string protocol;	 // List of accepted methods for the route (Not sure of name and variabel type)
	std::string redirection; // HTTP redirection
	std::string dirRoot;	 // not sure
	bool dirListing;		 // Enable or disable listening of directory
	std::string defaultFile; // default file when the request ressource is a directory
	bool uploadFiles;		 // Enable or disable uploading files from the clients to the server is authorized, and storage location is provided
	std::string CGI;		 // path of the CGI (maybe need more rule)
};

class Config
{
  private:
	std::string address;					  // IP address or localhost
	unsigned int port;						  // port (maybe can be a array)
	std::string errorPage;					  // Path of default error page (maybe needed error code)
	unsigned int maxSize;					  // max size bodies request
	std::map<std::string, struct site> sites; // key = site name, value = struct of element

  public:
	Config(std::string path);
	~Config();

	// GETTER
	std::string getAddress();
	const unsigned int getPort();
	const std::string getErrorPage();
	const unsigned int getMaxSize();
	const

		// GETTER FROM struct site
		std::string getProtocol(std::string siteName);
	const std::string getRedirection(std::string siteName);
	const std::string getDirRoot(std::string siteName);
	const bool getDirListing(std::string siteName);
	const std::string getDefaultFile(std::string siteName);
	const bool getUploadFiles(std::string siteName);
	const std::string getCGI(std::string siteName);
	const
};

#endif
