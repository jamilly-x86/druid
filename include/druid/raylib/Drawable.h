#pragma once

#include <cmath>
#include <vector>
#include <variant>
#include <cstdint>

namespace druid::raylib
{
	struct Vertex
	{
		float x{};
		float y{};
	};

	struct Color
	{
		std::uint8_t r{};
		std::uint8_t g{};
		std::uint8_t b{};
		std::uint8_t a{};
	};

	struct Rectangle
	{
		int x{};
		int y{};
		int width{};
		int height{};
		Color color{};
	};

	struct TriangleStrip
	{
		static TriangleStrip make_arc(float x, float y, float radius, float angle_start, float angle_end, float thickness, int segments, Color color)
		{
			TriangleStrip strip{};
			strip.color = color;
			const float angle_step = (angle_end - angle_start) / static_cast<float>(segments - 1);
			for (int i = 0; i < segments; ++i)
			{
				const float angle = angle_start + i * angle_step;
				const float rad = angle * (3.14159265f / 180.0f);
				const float outer_x = x + (radius + thickness / 2.0f) * std::cos(rad);
				const float outer_y = y + (radius + thickness / 2.0f) * std::sin(rad);
				const float inner_x = x + (radius - thickness / 2.0f) * std::cos(rad);
				const float inner_y = y + (radius - thickness / 2.0f) * std::sin(rad);
				strip.vertices.push_back({outer_x, outer_y});
				strip.vertices.push_back({inner_x, inner_y});
			}
			return strip;
		}

		std::vector<Vertex> vertices{};
		Color color{};
	};

	struct Arc
	{
		float x{};
		float y{};
		float radius{};
		float angle_start{};
		float angle_end{};
		int segments{};
		Color color{};
	};

	using Drawable = std::variant<Rectangle, TriangleStrip, Arc>;
}