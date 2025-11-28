module;

#include <raylib.h>

export module druid.raylib.rectangle;

namespace druid::raylib
{
	export struct Rectangle
	{
		int x{};
		int y{};
		int width{};
		int height{};
		Color color{};
	};
}