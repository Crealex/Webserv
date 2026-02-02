#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../includes.hpp"
# include "methodsClass.hpp"
# include "Post.hpp"
# include "Get.hpp"
# include "Delete.hpp"

class Request {

	private:

		static std::vector<std::string> _v;
		static std::vector<std::string> _acceptedType();
		
		std::string		_method;
		std::string		_location;
		std::string		_protocol;
		std::string		_host;
		std::string		_userAgent;
		std::string		_accept;
		std::string		_transferEncoding;
		std::string		_ContentType;
		unsigned int	_ContentLength;
		std::string		_body;
		size_t			_bodySize;
		bool			_keepAlive;

		void _checkPost();
		void _checkGet();

		void setFirstLine(std::string &line);
		std::string retBody(std::string &str);
		std::map<std::string, std::string*> createMap();
		
	public:

		Request();
		Request(const Request& cpy);
		Request& operator=(const Request& src);
		~Request();

		void printRequest();
		void parseHeader(std::string &buffer);
		void parseBody(std::string &buffer);
		void checkRequest(size_t maxSize);
		void reset();

		bool		getkeepAlive() const;
		std::string	getMethod() const;
		std::string	getLocation() const;
		std::string	getProtocol() const;
		std::string	getHost() const;
		std::string	getUserAgent() const;
		std::string	getAccept() const;
		std::string getTranferEncoding() const;
		std::string	getContentType() const;
		std::string	getBody() const;
		unsigned int getContentLength() const;

		void		setkeepAlive(bool b);
		void		setMethod(std::string str);
		void		setLocation(std::string str);
		void		setProtocol(std::string str);
		void		setHost(std::string str);
		void		setUserAgent(std::string str);
		void		setAccept(std::string str);
		void		setTranferEncoding(std::string str);
		void		setContentType(std::string str);
		void		setBody(std::string str);
		void		setContentLength(unsigned int n);	
};

#endif
