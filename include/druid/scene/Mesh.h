#pragma once

#include <vector>

namespace druid::scene
{
	struct Mesh
	{
		std::vector<float> vertices{};
		std::vector<uint32_t> indices{};
	};
}