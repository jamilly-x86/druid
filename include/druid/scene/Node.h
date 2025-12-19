#pragma once

#include <druid/scene/Transform.h>
#include <flecs.h>
#include <gsl/pointers>

namespace druid::scene
{
	class Tree;

	/// @class Node
	/// @brief Scene node wrapper around a Flecs entity, managed within a Tree.
	///
	/// `Node` represents a node in a hierarchical scene structure. It is bound to:
	/// - A owning/manager `Tree` (non-null)
	/// - A `flecs::entity` that stores the node's data (e.g., transform, mesh, material)
	///
	/// The node supports basic hierarchy edits (add/remove child) and uses a
	/// "dirty state" mechanism to signal which aspects of the node require
	/// recomputation (e.g., transform propagation, render data rebuild).
	class Node
	{
	public:
		/// @enum Dirty
		/// @brief Discrete dirty flags indicating which parts of a node need to be reprocessed.
		///
		/// These values are used to mark pending work such as transform propagation,
		/// mesh/material updates, or hierarchy changes. The `Tree` is typically
		/// responsible for responding to dirty states and performing the required updates.
		enum class Dirty : std::uint8_t
		{
			/// @brief Transform data changed (local/global recomputation needed).
			Transform,
			/// @brief Mesh data changed (geometry/buffers may need rebuilding).
			Mesh,
			/// @brief Material data changed (shader params/textures may need updating).
			Material,
			/// @brief A node was added to the hierarchy.
			NodeAdded,
			/// @brief A node was removed from the hierarchy.
			NodeRemoved
		};

		/// @brief Constructs a Node bound to a tree and a Flecs entity.
		/// @param x Reference to the Tree that will contain or manage the node.
		/// @param e flecs::entity representing the entity associated with the node.
		Node(Tree& x, flecs::entity e);

		/// @brief Destroy the node wrapper.
		///
		/// @note The destructor does not imply entity destruction unless the implementation
		///       explicitly performs such cleanup through `Tree`/Flecs.
		~Node();

		/// @brief Copy construction (shallow copy of tree pointer and entity handle).
		Node(const Node&) = default;

		/// @brief Move construction (moves tree pointer and entity handle).
		Node(Node&&) noexcept = default;

		/// @brief Copy assignment (shallow copy of tree pointer and entity handle).
		/// @return Reference to this node.
		[[nodiscard]] auto operator=(const Node&) -> Node& = default;

		/// @brief Move assignment (moves tree pointer and entity handle).
		/// @return Reference to this node.
		[[nodiscard]] auto operator=(Node&&) noexcept -> Node& = default;

		/// @brief Sets the node's Transform. Marks the Node's Transform as dirty.
		/// @param x The new Transform to set.
		auto set_transform(const Transform& x) -> void;

		/// @brief Retrieves the node's Transform. A transform is added to the node if it does not already exist.
		/// @return The node's Transform.
		[[nodiscard]] auto get_transform() const -> const Transform&;

		/// @brief Adds the specified node as a child. Does not take ownership.
		/// @param x Reference to the node to add as a child.
		auto add_child(Node& x) -> void;

		/// @brief Removes the specified child node from this parent. Does not take ownership.
		/// @param x Reference to the child node to remove. The parent or container that owns this node will be updated to no longer include it.
		auto remove_child(Node& x) -> void;

		/// @brief Marks the specified state as dirty.
		/// @param dirty The state to be marked as dirty.
		auto make_dirty(Dirty dirty) -> void;

	private:
		gsl::strict_not_null<Tree*> tree_;
		flecs::entity entity_;
	};
}