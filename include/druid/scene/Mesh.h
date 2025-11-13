#pragma once

#include <vector>

namespace druid::scene
{
	/// @brief Internal Node component for storing mesh rendering data.
	struct Mesh
	{
		std::vector<float> vertices{};
		std::vector<uint32_t> indices{};
	};
}
