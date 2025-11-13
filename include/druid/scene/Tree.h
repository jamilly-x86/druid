#pragma once

#include <druid/scene/Node.h>
#include <flecs.h>
#include <set>
#include <unordered_map>
#include <map>

namespace druid::scene
{
	/// @brief Manages nodes, tracks which nodes are dirty, and updates node transforms within a flecs world.
	class Tree
	{
	public:
		/// @brief Creates and returns a new Node wrapped in a std::unique_ptr.
		/// @return A std::unique_ptr<Node> that owns the newly created Node.
		[[nodiscard]] auto create_node() -> std::unique_ptr<Node>;

		/// @brief Marks the specified entity with the given dirty state for a node.
		/// @param dirty A Node::Dirty value (enum or flags) that specifies the dirty state to apply.
		/// @param e The flecs::entity to mark as dirty.
		auto make_dirty(Node::Dirty dirty, flecs::entity e) -> void;

		/// @brief Recalculates and applies object transforms (for example position, rotation, or scale) used by the system.
		auto update_transforms() -> void;

	private:
		flecs::world world_;

		// Look to replace with flat_map and flat_set for better iteration performance.
		// However, these data structures have higher insertion/removal cost, which
		// may be detrimental if there are many nodes being updated each frame.
		std::map<int, std::set<flecs::entity>> dirty_transforms_;
	};
}
