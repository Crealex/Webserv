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
	std::string							_maxSize;		// max size body's request
	std::map<unsigned int, std::string>	_errorPage;		// all the error codes
	std::vector<Location>				_locations;
	std::pair<std::string, std::string>	_cgiHandler;

	std::vector<std::string>	_getValue(std::string data);
	bool						_isValidDigit(std::string str);
	unsigned int				_checkDigitValue(std::string str);

	std::string	_parseHostname(std::string data);

	std::string _parseRoot(std::string data);

	std::string				_keywordAddress(std::string word);
	std::string				_checkAddress(std::string addressPort, size_t colon);
	bool					_isDuplicate(std::string add, unsigned int port, std::vector<addPort_t> res);
	std::vector<addPort_t>	_parseAddressPort(std::vector<std::string> data);
	
	std::map<unsigned int, std::string>	_parseErrorPage(std::vector<std::string> data);

	void			_checkMetricPrefix(std::vector<std::string> &infos);
	std::string	_parseMaxSize(std::string data);

	void	_parseElt(struct server data);

	void	_printAtt() const;

  public:
	Server();
	Server(struct server data);
	~Server();

	Server	&operator=(const Server &other);

	// GETTER
	std::string const							&getHostname() const;
	std::vector<addPort_t> const				&getAddressPort() const;
	std::string const							&getRoot() const;
	std::string const							&getMaxSize() const;
	std::map<unsigned int, std::string> const	&getErrorPage() const;
	std::vector<Location> const					&getLocations() const;
	std::pair<std::string, std::string>	const	&getCgiHandler() const;
};

#endif
