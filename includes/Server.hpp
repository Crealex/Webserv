#ifndef SERVER_HPP

#define SERVER_HPP

#include "includes.hpp"
#include "configStruct.hpp"
#include "socket/SocketData.hpp"
#include "Location.hpp"

typedef std::pair<std::string, unsigned int> addPort_t;

class Server
{
  private:
	std::string							_hostname;
	std::vector<addPort_t>				_addressPort;	// port (maybe can be a array)
	std::string							_root;
	unsigned int						_maxSize;		// max size body's request
	std::map<unsigned int, std::string>	_errorPage;		// all the error codes
	std::vector<Location>				_locations;
	std::pair<std::string, std::string>	_cgiHandler;

	void	parseElt(struct server data);

	void printAtt() const;

  public:
	Server(struct server data);
	~Server();

	// GETTER
	std::string const							&getHostname() const;
	std::vector<addPort_t> const				&getAddressPort() const;
	std::string const							&getRoot() const;
	unsigned int const							&getMaxSize() const;
	std::map<unsigned int, std::string> const	&getErrorPage() const;
	std::vector<Location> const					&getLocations() const;
	std::pair<std::string, std::string>	const	&getCgiHandler() const;
};

#endif
