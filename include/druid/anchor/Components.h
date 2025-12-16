#pragma once

#include <flecs.h>
#include <cstdint>

import druid.core.components;

namespace druid::anchor
{
	/// @struct Anchor
	/// @brief ECS component describing spatial anchoring between entities.
	///
	/// `Anchor` defines a relationship where an entity is positioned relative to
	/// a target entity at a specified anchor point, optionally with an additional
	/// positional offset. This is commonly used for UI layout, HUD elements, or
	/// relative entity placement.
	///
	/// The component is intended for use with Flecs-based systems that resolve
	/// anchored positions each frame or during layout passes.
	struct Anchor
	{
		/// @enum Point
		/// @brief Bitmask representing anchor points on an entity.
		///
		/// Anchor points may be combined to represent corners. Systems should
		/// interpret these flags consistently when computing anchored transforms.
		enum class Point : std::uint8_t
		{
			/// @brief Represents the top edge.
			Top = 0b00000001,
			/// @brief Represents the bottom edge.
			Bottom = 0b00000010,
			/// @brief Represents the left edge.
			Left = 0b00000100,
			/// @brief Represents the right edge.
			Right = 0b00001000,
			/// @brief Represents the center point.
			Center = 0b00010000,
			/// @brief Represents the top-left corner.
			TopLeft = Top | Left,
			/// @brief Represents the top-right corner.
			TopRight = Top | Right,
			/// @brief Represents the bottom-left corner.
			BottomLeft = Bottom | Left,
			/// @brief Represents the bottom-right corner.
			BottomRight = Bottom | Right,
		};

		/// @struct Target
		/// @brief Describes the anchor target entity and its anchor point.
		///
		/// The anchored entity will align its own `point` with the target entity's
		/// specified `point`.
		struct Target
		{
			/// @brief Target entity to anchor against.
			flecs::entity entity;
			/// @brief Anchor point on the target entity.
			Point point{Point::Center};
		};

		/// @brief Convenience helper to create a center-anchor target.
		///
		/// @param e Target entity to anchor against.
		/// @return Target describing a center-to-center anchor.
		static auto center(flecs::entity e) -> Target
		{
			return Target{.entity = e, .point = Point::Center};
		}

		/// @brief Anchor point on this entity.
		///
		/// This point on the current entity will be aligned with the target's
		/// anchor point.
		Point point{Point::Center};
		/// @brief Target entity and anchor point.
		Target target{};
		/// @brief Positional offset applied after anchoring.
		///
		/// This offset is applied in the same coordinate space as the resolved
		/// anchor position.
		core::Position offset{};
	};
}
