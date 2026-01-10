module;

#include <functional>

export module briarthorn.Test;

export template <typename T>
struct Test
{
	std::function<T> test_;
};