#pragma once

namespace druid::scene
{
	/// @brief Internal Node component used to track heirarchical metadata such as node depth.
	struct Heirarchy
	{
		int depth{};
	};
}
