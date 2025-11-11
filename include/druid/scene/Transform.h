#pragma once

#include <glm/mat4x4.hpp>

namespace druid::scene
{
	struct Transform
	{
		glm::mat4 global{};
		glm::mat4 local{};
	};
}