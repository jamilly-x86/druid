#pragma once

#include <flecs.h>
#include <cstdint>

import druid.core.components;

namespace druid::anchor
{
	struct Anchor
	{
		enum class Point : std::uint8_t
		{
			Top = 0b00000001,
			Bottom = 0b00000010,
			Left = 0b00000100,
			Right = 0b00001000,
			Center = 0b00010000,
			TopLeft = Top | Left,
			TopRight = Top | Right,
			BottomLeft = Bottom | Left,
			BottomRight = Bottom | Right,
		};

		struct Target
		{
			flecs::entity entity;
			Point point{Point::Center};
		};

		static auto center(flecs::entity e) -> Target
		{
			return Target{.entity = e, .point = Point::Center};
		}

		Point point{Point::Center};
		Anchor::Target target{};
		druid::core::Position offset{};
	};
}
