#ifndef SERVER_HPP

#define SERVER_HPP

#include "includes.hpp"

typedef std::pair<std::string, unsigned int> addPort_t;

class Config
{
  private:
	std::string							hostname;
	std::vector<addPort_t>				addressPort;	// port (maybe can be a array)
	std::string							root;
	unsigned int						maxSize;		// max size bodies request
	std::map<unsigned int, std::string>	errorPage;		// all the error codes
	std::vector<Location>				locations;

	void 	siteParsing(struct siteParse site);
	void	parseElt(struct structParse);

  public:
	Config(std::string pathConfig);
	~Config();

	
	// GETTER
	std::string const							&getHostname() const;
	std::vector<addPort_t> const				&getAddressPort() const;
	std::string const							&getRoot() const;
	unsigned int								&getMaxSize() const;
	std::map<unsigned int, std::string> const	&getErrorPage() const;
	std::vector<Location> const					&getSitesName() const;
};

#endif
