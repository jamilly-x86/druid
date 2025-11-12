#pragma once

#include <glm/mat4x4.hpp>

namespace druid::scene
{
	struct Transform
	{
		glm::mat4 global{1.0F};
		glm::mat4 local{1.0F};
	};
}