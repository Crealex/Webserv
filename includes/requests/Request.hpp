#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../includes.hpp"
# include "Method.hpp"
# include "Post.hpp"
# include "Get.hpp"
# include "Delete.hpp"

enum	statusType
{
	NOTHING,
	SETTIMEOUT,
	PARSINGHEADERDONE,
	READY,
};

class Request {

	private:

		static std::vector<std::string>	_v;
		static std::vector<std::string>	_acceptedType();
		
		std::string		_method;
		std::string		_location;
		std::string		_protocol;
		std::string		_host;
		std::string		_userAgent;
		std::string		_accept;
		std::string		_transferEncoding;
		std::string		_ContentType;
		unsigned int	_ContentLength;
		std::string		_URI;
		std::string		_query;
		std::string		_body;
		std::string		_bodySize;
		bool			_keepAlive;
		bool			_error;
		bool			_firstLine;
		bool			_header;
		enum statusType	_status;
		std::fstream	_file;

		void	_parseAccept();
		void	_checkPost();
		void	_checkGet();

		void								setFirstLine(std::string &line);
		void								parseHeader(std::string &buffer);
		std::string							retBody(std::string &str);
		std::map<std::string, std::string*>	createMap();
		
	public:

		Request();
		Request(const Request& cpy);
		Request& operator=(const Request& src);
		~Request();

		std::string	getRawRequest() const;
		void		parseBody(std::string &buffer);
		void		startParse(std::string &buffer);
		void		checkRequest(const std::string maxSize);
		void		reset();

		bool			getkeepAlive() const;
		bool			getHeader() const;
		std::string		getMethod() const;
		std::string		getLocation() const;
		std::string		getProtocol() const;
		std::string		getHost() const;
		std::string		getUserAgent() const;
		std::string		getAccept() const;
		std::string 	getTranferEncoding() const;
		std::string		getContentType() const;
		std::string		getURI() const;
		std::string		getQuery() const;
		std::string		getBody() const;
		unsigned int	getContentLength() const;
		std::string		getStrContentLength() const;
		enum statusType	getStatus() const;
		int				getFile() const;

		void	setkeepAlive(bool b);
		void	setMethod(std::string str);
		void	setLocation(std::string str);
		void	setProtocol(std::string str);
		void	setHost(std::string str);
		void	setUserAgent(std::string str);
		void	setAccept(std::string str);
		void	setTranferEncoding(std::string str);
		void	setContentType(std::string str);
		void	setBody(std::string str);
		void	setContentLength(unsigned int n);	
		void	setStatus(enum statusType newStatus);	
		void	setFile(int newFile);
};

#endif
