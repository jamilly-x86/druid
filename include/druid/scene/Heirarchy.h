#pragma once

namespace druid::scene
{
	/// @struct Heirarchy
	/// @brief Internal scene-graph hierarchy metadata component.
	///
	/// Stores metadata derived from a node’s position within the scene hierarchy.
	/// This component is typically maintained by scene systems and not modified
	/// directly by user code.
	struct Heirarchy
	{
		/// @brief Depth of the node within the scene tree.
		///
		/// The root node typically has depth 0, with each successive child
		/// increasing the depth by one.
		int depth{};
	};
}
