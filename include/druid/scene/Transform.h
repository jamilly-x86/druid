#pragma once

#include <glm/mat4x4.hpp>

namespace druid::scene
{
	/// @struct Transform
	/// @brief Internal scene-graph transform component.
	///
	/// Stores both local and global transformation matrices used during scene
	/// traversal and rendering. This component is typically managed by scene
	/// systems and not manipulated directly by user code.
	///
	/// The `local` transform represents the node's transformation relative to
	/// its parent, while the `global` transform represents the fully composed
	/// transformation in world space.
	struct Transform
	{
		/// @brief Global (world-space) transformation matrix.
		///
		/// Computed by composing the node's local transform with all ancestor
		/// transforms up to the root.
		glm::mat4 global{1.0F};
		/// @brief Local (parent-relative) transformation matrix.
		///
		/// Represents translation, rotation, and scale relative to the parent node.
		glm::mat4 local{1.0F};
	};
}
