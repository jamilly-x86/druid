#pragma once

#include <vector>

namespace druid::scene
{
	/// @struct Mesh
	/// @brief Internal scene-graph mesh component.
	///
	/// Stores raw geometry data required for rendering a mesh. This component is
	/// intended for internal use by scene and rendering systems and does not
	/// encode any ownership or lifetime semantics beyond its data containers.
	///
	/// The interpretation of vertex layout (position, normals, UVs, etc.) and
	/// index topology is defined by the rendering pipeline that consumes this
	/// component.
	struct Mesh
	{
		/// @brief Vertex buffer data.
		///
		/// Stored as a flat array of floating-point values. The exact layout
		/// (e.g., position-only, position + normal + UV) is renderer-dependent.
		std::vector<float> vertices{};

		/// @brief Index buffer data.
		///
		/// Defines the order in which vertices are assembled into primitives
		/// (typically triangles).
		std::vector<uint32_t> indices{};
	};
}
