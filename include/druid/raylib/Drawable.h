#pragma once

#include <cstdint>
#include <variant>
#include <vector>

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

	static constexpr auto Black = Color{.r=0, .g=0, .b=0, .a=255};

	struct Rectangle
	{
		int x{};
		int y{};
		int width{};
		int height{};
		Color color{};
	};

	struct Arc
	{
		float x{};
		float y{};
		float radius{};
		float angle_start{};
		float angle_end{};
		float line_width{};
		int segments{};
		Color color{};
	};

	struct TriangleStrip
	{
		static auto from_arc(druid::raylib::Arc x) -> TriangleStrip;

		std::vector<Vertex> vertices;
		Color color{};
	};

	using Drawable = std::variant<Rectangle, TriangleStrip, Arc>;
}