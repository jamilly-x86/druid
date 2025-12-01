#pragma once

#include <string>

namespace druid::raylib
{
	struct Window
	{
		std::string title;
		int x{};
		int y{};
		int width{};
		int height{};
	};
}
