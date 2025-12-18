#pragma once

#include <cstdint>
#include <variant>
#include <vector>

namespace druid::raylib
{
	/// @struct Vertex
	/// @brief 2D vertex representation.
	///
	/// Represents a single point in 2D space, typically used for constructing
	/// primitive shapes such as triangle strips.
	struct Vertex
	{
		/// @brief X-coordinate in screen or world space.
		float x{};
		/// @brief Y-coordinate in screen or world space.
		float y{};
	};

	/// @struct Color
	/// @brief RGBA color representation.
	///
	/// Color channels are stored as 8-bit unsigned integers in RGBA order.
	struct Color
	{
		/// @brief Red channel.
		std::uint8_t r{};
		/// @brief Green channel.
		std::uint8_t g{};
		/// @brief Blue channel.
		std::uint8_t b{};
		/// @brief Alpha channel (opacity).
		std::uint8_t a{};
	};

	/// @brief Predefined opaque black color.
	static constexpr auto Black = Color{.r = 0, .g = 0, .b = 0, .a = 255};

	/// @struct Rectangle
	/// @brief Axis-aligned rectangle drawable.
	///
	/// Represents a filled rectangle with a position, size, and color.
	/// Interpretation of the position (top-left vs center) is determined
	/// by the rendering backend.
	struct Rectangle
	{
		/// @brief X-coordinate of the rectangle origin.
		int x{};
		/// @brief Y-coordinate of the rectangle origin.
		int y{};
		/// @brief Rectangle width in pixels or units.
		int width{};
		/// @brief Rectangle height in pixels or units.
		int height{};
		/// @brief Fill color of the rectangle.
		Color color{};
	};

	/// @struct Arc
	/// @brief Circular arc drawable.
	///
	/// Represents a stroked arc segment defined by a center point, radius,
	/// angular range, and line width.
	struct Arc
	{
		/// @brief X-coordinate of the arc center.
		float x{};
		/// @brief Y-coordinate of the arc center.
		float y{};
		/// @brief Radius of the arc.
		float radius{};
		/// @brief Starting angle of the arc (in degrees).
		float angle_start{};
		/// @brief Ending angle of the arc (in degrees).
		float angle_end{};
		/// @brief Width of the arc stroke.
		float line_width{};
		/// @brief Number of segments used to approximate the arc.
		int segments{};
		/// @brief Color of the arc stroke.
		Color color{};
	};

	/// @struct TriangleStrip
	/// @brief Triangle strip drawable.
	///
	/// Represents a connected strip of triangles defined by a sequence of vertices.
	/// Useful for rendering complex filled shapes efficiently.
	struct TriangleStrip
	{
		/// @brief Vertices forming the triangle strip.
		std::vector<Vertex> vertices;
		/// @brief Fill color applied to the entire strip.
		Color color{};
	};

	/// @brief Variant type representing any drawable primitive.
	///
	/// Used to store heterogeneous draw commands that can be dispatched to
	/// the rendering backend via `std::visit`.
	using Drawable = std::variant<Rectangle, TriangleStrip, Arc>;
}
