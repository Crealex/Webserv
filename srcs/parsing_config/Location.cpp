#include "../../includes/Location.hpp"

/**
 * @brief Check if there is a semicolon at the end of the string 
 * and erase it, if not throw an error
 * 
 * @param str 
 */
static void removeSemiColon(std::string &str)
{
	std::string::iterator ite = str.end();
	ite--;
	if (*ite == ';')
		str.erase(ite);
	else
	{
		std::string error("Error: no semicolon on line:\n\t");
		throw std::invalid_argument(error + str);
	}
}

/**
 * @brief check if the number of element after the first in
 * the str string is equal to n. if not throw an error
 * 
 * @param n number of element wanted
 * @param str the string checked
 * @return true 
 * @return false 
 */
static bool checkNbrElt(size_t n, std::string str)
{
	std::stringstream ss(str);
	int i = 0;
	std::string check;
	while (ss >> check);
		i++;
	if (i != n)
	{
		std::string error("Error: wrong number of argument on line:\n\t");
		throw std::invalid_argument(error + str);
	}
	return true;
}

static bool retAutoIndex(std::string str)
{
	checkNbrElt(2, str);
	removeSemiColon(str);

	std::stringstream ss(str);
	str.clear();
	while (ss >> str)
	{
	}
	if (str == "false")
		return false;
	else
		return true;
}

static std::string retSecond(std::string str)
{
	checkNbrElt(2, str);
	removeSemiColon(str);

	std::stringstream ss(str);
	str.clear();
	while (ss >> str)
	{
	}
	return str;
}

static std::string retPath(std::string str, std::string root)
{
	checkNbrElt(3, str);
	std::stringstream ss(str);
	str.clear();
	ss >> str;
	ss >> str;

	str = root + str;
	if (access(str.c_str(), F_OK) == -1)
	{
		std::string error("Error: could not open Dir:\n\t");
		throw std::invalid_argument(error + str);
	}

	return str;
}

static std::vector<pairString> retCgi(std::vector<std::string> v)
{
	std::vector<pairString> ret;

	for (std::vector<std::string>::iterator it = v.begin();
		it != v.end(); it++)
	{
		checkNbrElt(2, *it);
		removeSemiColon(*it);
		std::stringstream ss(*it);
		std::string ext;
		std::string inter;

		ss >> ext;
		ss >> inter;

		ret.push_back(std::make_pair(ext, inter));
	}

	return ret;
}

static void checkIndex(std::string index, std::string path)
{
	std::ifstream ifs(path + index);

	if (!ifs.is_open())
	{
		std::string error("Error: could not open file:\n\t");
		throw std::invalid_argument(error + path + index);
	}
}

static void checkPath(std::string str)
{
	if (!access(str.c_str(), F_OK))
	{
		std::string error("Error: could not open Dir:\n\t");
		throw std::invalid_argument(error + str);
	}
}

Location::Location(location src, std::string root)
{
	_autoIndex = retAutoIndex(src.autoIndex);
	_ret = retSecond(src.ret);
	_uploadPath = retSecond(src.uploadPath);

	_path = retSecond(src.path);
	_index = retSecond(src.index);
	checkIndex(_index, _path);
	_cgiHandler = retCgi(src.cgi);
}

Location::Location(const Location& cpy)
{
	this->_autoIndex = cpy._autoIndex;
	this->_index = cpy._index;
	this->_ret = cpy._ret;
	this->_uploadPath = cpy._uploadPath;
	this->_path = cpy._path;
	this->_allowedMethods = cpy._allowedMethods;
	this->_cgiHandler = cpy._cgiHandler;
}

Location &Location::operator=(const Location& src)
{
	if (this != &src)
	{
		this->_autoIndex = src._autoIndex;
		this->_index = src._index;
		this->_ret = src._ret;
		this->_uploadPath = src._uploadPath;
		this->_path = src._path;
		this->_allowedMethods = src._allowedMethods;
		this->_cgiHandler = src._cgiHandler;
	}
	return *this;
}

Location::~Location()
{
	
}

bool Location::getAutoIndex() const
{
	return _autoIndex;
}

std::string Location::getIndex() const
{
	return _index;
}

std::string Location::getReturn() const
{
	return _ret;
}

std::string Location::getUploadPath() const
{
	return _uploadPath;
}

std::string Location::getPath() const
{
	return _path;
}

std::vector<std::string> Location::getAllowedMethods() const
{
	return _allowedMethods;
}

std::vector<pairString> Location::getCgiHandler() const
{
	return _cgiHandler;
}

std::vector<Location> createLocations(server serv)
{
	std::vector<Location> ret;

	for (std::vector<location>::iterator it = serv.locations.begin();
		it != serv.locations.end(); it++)
	{
		ret.push_back(Location(*it, serv.root));
	}

	return ret;
}
