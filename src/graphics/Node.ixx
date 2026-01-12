module;

#include <raylib.h>
#include <rlgl.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <type_traits>
#include <vector>

export module druid.graphics.Node;

import druid.core.Signal;
import druid.graphics.Renderer;

using druid::core::Signal;

export namespace druid::graphics
{
	class Node;

	/// @concept NodeType
	/// @brief Constrains a type to those derived from druid::graphics::Node.
	template <typename T>
	concept NodeType = std::is_base_of_v<Node, T>;

	/// @class Node
	/// @brief Scene-graph node with local transform and draw callback support.
	///
	/// `Node` provides spatial transform data (position/scale/rotation) and
	/// hierarchical transform composition. Nodes can be arranged in a tree;
	/// each node can compute its local transform and its global transform
	/// (including ancestors).
	///
	/// Rendering is performed via `draw(Renderer&)`, which typically invokes the
	/// `on_draw` signal so that node subclasses or external systems can attach
	/// render behavior without tightly coupling to the node type.
	///
	/// Each node owns its children via `std::unique_ptr`, ensuring clear ownership
	/// and automatic cleanup.
	class Node
	{
	public:
		/// @brief Default local position (0, 0).
		static constexpr auto DefaultPosition = glm::vec2{0.0F, 0.0F};
		/// @brief Default local scale (1, 1).
		static constexpr auto DefaultScale = glm::vec2{1.0F, 1.0F};
		/// @brief Default local rotation (0 radians or degrees depending on implementation).
		///
		/// @note This header does not specify the unit; the implementation should document
		///       whether rotation is in radians or degrees and remain consistent across systems.
		static constexpr auto DefaultRotation = 0.0F;

		/// @brief Construct a node.
		explicit Node() = default;

		/// @brief Virtual destructor for proper cleanup in hierarchies.
		virtual ~Node() noexcept = default;

		Node(const Node&) = delete;
		auto operator=(const Node&) -> Node& = delete;
		Node(Node&&) noexcept = delete;
		auto operator=(Node&&) noexcept -> Node& = delete;

		/// @brief Add a child node to this node.
		/// @param child The child node to add (ownership transferred).
		auto add_child(std::unique_ptr<Node> child) -> void
		{
			if (child == nullptr)
			{
				return;
			}

			child->parent_node_ = this;
			child->dirty();
			children_.emplace_back(std::move(child));
		}

		/// @brief Remove this node from its parent if it has one.
		/// @return The owning unique_ptr of this node, or nullptr if no parent.
		[[nodiscard]] auto remove() -> std::unique_ptr<Node>
		{
			if (parent_node_ == nullptr)
			{
				return nullptr;
			}

			auto it = std::ranges::find_if(parent_node_->children_, [this](const auto& node) { return this == node.get(); });

			if (it == std::end(parent_node_->children_))
			{
				return nullptr;
			}

			auto node = std::move(*it);
			auto* parent = parent_node_;
			parent_node_ = nullptr;
			dirty();
			parent->children_.erase(it);
			return node;
		}

		/// @brief Get the parent node.
		/// @return Pointer to parent node, or nullptr if this is a root node.
		[[nodiscard]] auto parent() const noexcept -> Node*
		{
			return parent_node_;
		}

		/// @brief Get the list of children this node owns.
		/// @return Read-only vector of child nodes.
		[[nodiscard]] auto children() const noexcept -> const std::vector<std::unique_ptr<Node>>&
		{
			return children_;
		}
		/// @brief Set the local position of this node.
		/// @param pos New local position.
		auto set_position(const glm::vec2& pos) -> void
		{
			position_ = pos;
			dirty();
		}

		/// @brief Get the local position of this node.
		/// @return Local position.
		[[nodiscard]] auto get_position() const -> glm::vec2
		{
			return position_;
		}

		/// @brief Set the local scale of this node.
		/// @param scale New local scale.
		auto set_scale(const glm::vec2& scale) -> void
		{
			scale_ = scale;
			dirty();
		}

		/// @brief Get the local scale of this node.
		/// @return Local scale.
		[[nodiscard]] auto get_scale() const -> glm::vec2
		{
			return scale_;
		}

		/// @brief Set the local rotation of this node.
		/// @param rotation New local rotation angle.
		auto set_rotation(float rotation) -> void
		{
			rotation_ = rotation;
			dirty();
		}

		/// @brief Get the local rotation of this node.
		/// @return Local rotation angle.
		[[nodiscard]] auto get_rotation() const -> float
		{
			return rotation_;
		}

		/// @brief Create and attach a child node of type `T`.
		///
		/// The child is owned by this node via unique_ptr.
		///
		/// @tparam T Node type (must derive from Node).
		/// @tparam Args Constructor argument types for T.
		/// @param args Arguments forwarded to the child's constructor.
		/// @return Reference to the created child node.
		template <NodeType T, typename... Args>
		[[nodiscard]] auto create_node(Args&&... args) -> T&
		{
			auto child = std::make_unique<T>(std::forward<Args>(args)...);
			auto* ptr = child.get();
			add_child(std::move(child));
			return *ptr;
		}

		/// @brief Create and attach a generic child node.
		/// @return Reference to the created child node.
		[[nodiscard]] auto create_node() -> Node&
		{
			return create_node<Node>();
		}

		/// @brief Get the local transform matrix.
		///
		/// The transform is derived from local position/scale/rotation.
		///
		/// @return Local transform matrix.
		[[nodiscard]] auto transform() const -> glm::mat4
		{
			if (transform_dirty_)
			{
				const auto translation = glm::translate(glm::mat4(1.0F), glm::vec3(position_.x, position_.y, 0.0F));
				const auto rotation = glm::rotate(glm::mat4(1.0F), glm::radians(rotation_), glm::vec3(0.0F, 0.0F, 1.0F));
				const auto scaling = glm::scale(glm::mat4(1.0F), glm::vec3(scale_.x, scale_.y, 1.0F));
				transform_ = translation * rotation * scaling;
				transform_dirty_ = false;
			}
			return transform_;
		}

		/// @brief Draw this node using the provided renderer.
		///
		/// Applies the local transform, emits the on_draw signal, and recursively
		/// draws all child nodes.
		///
		/// @param x Renderer used for issuing draw calls.
		auto draw(Renderer& x) const -> void
		{
			rlPushMatrix();
			rlMultMatrixf(glm::value_ptr(transform_));

			on_draw_(x);

			for (const auto& child : children_)
			{
				child->draw(x);
			}

			rlPopMatrix();
		}

		/// @brief Subscribe to this node's draw callback.
		///
		/// The callback is invoked when `draw(Renderer&)` is executed for this node.
		///
		/// @tparam Callback A callable type invocable with `Renderer&`.
		/// @param x Callback function to invoke when the node is drawn.
		template <typename Callback>
		auto on_draw(Callback&& x) -> void
		{
			on_draw_.connect(std::forward<Callback>(x));
		}

	private:
		/// @brief Mark transforms as dirty and invalidate children.
		auto dirty() -> void
		{
			transform_dirty_ = true;
			for (auto& child : children_)
			{
				child->dirty();
			}
		}

		std::vector<std::unique_ptr<Node>> children_;
		Node* parent_node_{nullptr};
		Signal<void(Renderer&)> on_draw_;
		mutable glm::mat4 transform_{glm::mat4(1.0F)};
		glm::vec2 position_{DefaultPosition};
		glm::vec2 scale_{DefaultScale};
		float rotation_{DefaultRotation};
		mutable bool transform_dirty_{true};
	};
}
