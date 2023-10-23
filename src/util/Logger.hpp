#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

namespace Logger
{
	template <typename T>
	void log(T value)
	{
		std::cout << value << std::endl;
	}
}

#endif 