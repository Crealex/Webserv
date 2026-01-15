#include "../../includes/Location.hpp"

/**
 * @brief check if the number of element after the first in
 * the str string is equal to n. if not throw an error
 * 
 * @param n number of element wanted
 * @param str the string checked
 * @return true 
 * @return false 
 */
static void checkNbrElt(size_t n, std::string str)
{
	std::stringstream ss(str);
	unsigned int i = 0;
	std::string check;
	while (ss >> check)
		i++;
	if (i != n)
	{
		std::string error("Error: wrong number of argument on line:\n\t");
		throw std::invalid_argument(error + str);
	}
}

static bool retAutoIndex(std::string str)
{
	if (str.empty())
		return false;
	checkNbrElt(2, str);

	std::stringstream ss(str);
	str.clear();
	while (ss >> str)
	{
	}
	if (str == "off")
		return false;
	else if (str == "on")
		return true;
	throw std::invalid_argument("Error, wrong argument for autoIndex on line:\n" + str);
}

static std::string retSecond(std::string str, size_t n)
{
	if (str.empty())
		return str;
	checkNbrElt(n, str);

	std::stringstream ss(str);
	str.clear();

	ss >> str;
	ss >> str;

	return str;
}

//static std::string retPath(std::string str, std::string root)
//{
//	if (str.empty())
//		return str;
//	checkNbrElt(2, str);
//	std::stringstream ss(str);
//	str.clear();
//	ss >> str;
//	ss >> str;
//
//	str = root + str;
//	if (access(str.c_str(), R_OK) == -1)
//	{
//		std::string error("Error: could not open Dir:\n\t");
//		throw std::invalid_argument(error + str);
//	}
//
//	return str;
//}

static std::map<pairString> retCgi(std::vector<std::string> v)
{
	std::map<pairString> ret;
	if (v.empty())
		return ret;
	
	for (std::vector<std::string>::iterator it = v.begin();
	it != v.end(); it++)
	{
		checkNbrElt(3, *it);
		std::stringstream ss(*it);
		std::string ext;
		std::string inter;

		ss >> ext;
		ss >> ext;
		ss >> inter;

		ret.insert(std::make_pair(ext, inter));
	}

	return ret;
}

static std::pair<std::string, size_t> retReturn(std::string str)
{
	if (str.empty())
		return std::make_pair("", 0);
	checkNbrElt(3, str);
	std::stringstream ss(str);
	size_t		status;
	std::string	redir;

	ss >> redir;
	ss >> status;
	ss >> redir;

	return std::make_pair(redir, status);
}

static void checkIndex(std::string index, std::string path)
{
	if (index.empty())
		return ;
	if (index.compare(index.size() - 5, 5, ".html"))
	{
		std::string error("Error: wrong index extension\n\t");
		throw std::invalid_argument(error + path + index);
	}

	std::ifstream ifs(path + index);

	if (!ifs.is_open())
	{
		std::string error("Error: could not open file:\n\t");
		throw std::invalid_argument(error + path + index);
	}
}

std::map<std::string, bool> retMethods(std::string str)
{
	std::map<std::string, bool> ret;
	ret["GET"] = false;
	ret["POST"] = false;
	ret["DELETE"] = false;
	if (str.empty())
		return ret;

	std::stringstream ss(str);
	std::string word;
	ss >> word;
	while (ss >> word)
	{
		try
		{
			ret.at(word) = true;
		}
		catch(...)
		{
			std::string error("Error, wrong Method on line:\n");
			throw std::invalid_argument(error + str);
		}
	}

	return ret;
}

Location::Location(location src, std::string root)
{
	_autoIndex = retAutoIndex(src.autoIndex);
	_ret = retReturn(src.ret);
	_uploadPath = retSecond(src.uploadPath, 2);
	_path = retSecond(src.path, 3);
	_index = retSecond(src.index, 2);
	checkIndex(_index, retSecond(root, 2) + _path);
	_allowedMethods = retMethods(src.allowedMethods);
	_cgiHandler = retCgi(src.cgi);
	checkValidity();
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

std::pair<std::string, size_t> Location::getReturn() const
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

bool Location::getMethodValue(std::string key) const
{
	return _allowedMethods.at(key);
}

std::map<std::string, bool> Location::getAllowedMethods() const
{
	return _allowedMethods;
}

std::map<pairString> Location::getCgiHandler() const
{
	return _cgiHandler;
}

void Location::print() const
{
	std::cout << "Location "  << BOLD << _path << RESET << std::endl
		<< "\tautoIndex = " << std::boolalpha << _autoIndex
		<< "\n\tindex = " << _index
		<< "\n\tret = " << _ret.first << ", " << _ret.second
		<< "\n\tuploadPath = " << _uploadPath
		<< "\n\tallowedMethods = " << _allowedMethods
		<< "\n\tcgi = " << _cgiHandler << std::endl;
}

void Location::checkValidity()
{
	if (_ret.first.empty() ^ // XOR condition it accept if only one of the 2 element are true
			(_allowedMethods.at("GET") == true || 
			_allowedMethods.at("POST") == true ||
			_allowedMethods.at("DELETE") == true))
	{
		throw std::invalid_argument("Error, redirection and method on the location or non of them: " + _path);
	}

	if (!_index.empty() && _autoIndex)
	{
		throw std::invalid_argument("Error, index and auto index on the location: " + _path);
	}
}

std::vector<Location> createLocations(server serv)
{
	std::vector<Location> ret;

	for (std::vector<location>::iterator it = serv.locations.begin();
		it != serv.locations.end(); it++)
	{
		ret.push_back(Location(*it, serv.root));
	}

	std::cout << serv.root + '/' << std::endl;
	int check = 0;
	for (std::vector<Location>::iterator it = ret.begin();
		it != ret.end(); it++)
	{
		if (it->getPath() == "/")
			check = 1;
		for (std::vector<Location>::iterator it2 = it + 1;
			it2 != ret.end(); it2++)
		{
			if (it->getPath() == it2->getPath())
				throw std::invalid_argument("Error, multiple location with same path:\n\t" + it->getPath());
		}
	}
	if (check == 0)
		throw std::invalid_argument("Error, no '/' root location in config file.");

	return ret;
}

std::ostream& operator<<(std::ostream &os, std::map<pairString> map)
{
	std::map<pairString>::iterator it = map.begin();

	while (it != map.end())
	{
		os << "first = " << it->first << ",second = " << it->second;
		os << "  -  ";
		it++;
	}
	os << std::endl;

	return os;
}

std::ostream& operator<<(std::ostream &os, std::map<std::string, bool> map)
{
	std::map<std::string, bool>::iterator it = map.begin();

	while (it != map.end())
	{
		os << std::boolalpha << "method = " << it->first << ",is allowed = " << it->second;
		os << "  -  ";
		it++;
	}
	os << std::endl;

	return os;
}
