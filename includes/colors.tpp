#include "colors.hpp"

template <typename T>
void writeColors(T input, char *colors)
{
	std::cout << colors << input;
}

template <typename T>
void writeStyle(T input, char *style)
{
	std::cout << style << input;
}

template <typename T>
void writeColorsStyle(T input, char *colors, char *style)
{
	std::cout << colors << style << input;
}
