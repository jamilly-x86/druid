#pragma once

#include <druid/scene/Transform.h>
#include <flecs.h>
#include <gsl/pointers>

namespace druid::scene
{
	class Tree;

	/// @brief Represents a scene node that stores transforms, mesh, and material for rendering within a hierarchical tree.
	class Node
	{
	public:
		/// @brief Represents discrete kinds of changes (dirty states) that indicate which part of a node needs to be re-calculated.
		enum class Dirty : std::uint8_t
		{
			Transform,
			Mesh,
			Material,
			NodeAdded,
			NodeRemoved
		};

		/// @brief Constructs a Node bound to a tree and a Flecs entity.
		/// @param x Reference to the Tree that will contain or manage the node.
		/// @param e flecs::entity representing the entity associated with the node.
		Node(Tree& x, flecs::entity e);
		~Node();

		Node(const Node&) = default;
		Node(Node&&) noexcept = default;

		[[nodiscard]] auto operator=(const Node&) -> Node& = default;
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