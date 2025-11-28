module;

#include <string>

export module druid.raylib.window;

namespace druid::raylib
{
	export struct Window
	{
		std::string title;
		int x{};
		int y{};
		int width{};
		int height{};
	};
}