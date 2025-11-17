#ifndef CONFIG_HPP

#define CONFIG_HPP

#include <iostream>

class Config
{
  private:
  public:
	Config();
	Config(const Config &cpy);
	~Config();
	Config &operator=(const Config &rhs);
};

#endif // !CONFIG_HPP
