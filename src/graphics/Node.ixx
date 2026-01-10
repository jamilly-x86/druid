module;

#include <raylib.h>
#include <rlgl.h>
#include <memory>
#include <type_traits>
#include <vector>

export module druid.graphics.Node;

import druid.core.Object;
import druid.core.Engine;
import druid.core.Signal;
import druid.graphics.Renderer;
import druid.math.Vec2;
import druid.math.Vec3;
import druid.math.Mat4;

using druid::core::Engine;
using druid::core::Signal;
using druid::math::Mat4;
using druid::math::Vec2;
using druid::math::Vec3;

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
	/// `Node` extends `druid::core::Object` to provide spatial transform data
	/// (position/scale/rotation) and hierarchical transform composition.
	/// Nodes can be arranged in a tree; each node can compute its local transform
	/// and its global transform (including ancestors).
	///
	/// Rendering is performed via `draw(Renderer&)`, which typically invokes the
	/// `on_draw` signal so that node subclasses or external systems can attach
	/// render behavior without tightly coupling to the node type.
	///
	/// @note `Node` hides `Object::create_child` to ensure children are also `Node`s.
	class Node : public core::Object
	{
	public:
		/// @brief Default local position (0, 0).
		static constexpr auto DefaultPosition = Vec2{0.0F, 0.0F};
		/// @brief Default local scale (1, 1).
		static constexpr auto DefaultScale = Vec2{1.0F, 1.0F};
		/// @brief Default local rotation (0 radians or degrees depending on implementation).
		///
		/// @note This header does not specify the unit; the implementation should document
		///       whether rotation is in radians or degrees and remain consistent across systems.
		static constexpr auto DefaultRotation = 0.0F;

		/// @brief Construct a node associated with the given engine.
		/// @param x Owning engine instance.
		Node(druid::core::Engine& x) : Object{x}
		{
			on_added([this](auto* parent) { parent_node_ = dynamic_cast<Node*>(parent); });
			on_removed([this](auto*) { parent_node_ = nullptr; });
			on_child_added([this](auto* child) { nodes_.emplace_back(dynamic_cast<Node*>(child)); });
			on_child_removed([this](auto* child) { std::erase(nodes_, child); });
		}

		/// @brief Set the local position of this node.
		/// @param pos New local position.
		auto set_position(const Vec2& pos) -> void
		{
			position_ = pos;
			update_transform();
		}

		/// @brief Get the local position of this node.
		/// @return Local position.
		[[nodiscard]] auto get_position() const -> Vec2
		{
			return position_;
		}

		/// @brief Get the global position of this node.
		///
		/// Computed from the global transform and represents the node's position
		/// in world space (including parent transforms).
		///
		/// @return Global/world position.
		[[nodiscard]] auto get_position_global() const -> Vec2
		{
			const auto& t = transform_global();
			return {t[3][0], t[3][1]};
		}

		/// @brief Set the local scale of this node.
		/// @param scale New local scale.
		auto set_scale(const Vec2& scale) -> void
		{
			scale_ = scale;
			update_transform();
		}

		/// @brief Get the local scale of this node.
		/// @return Local scale.
		[[nodiscard]] auto get_scale() const -> Vec2
		{
			return scale_;
		}

		/// @brief Set the local rotation of this node.
		/// @param rotation New local rotation angle.
		auto set_rotation(float rotation) -> void
		{
			rotation_ = rotation;
			update_transform();
		}

		/// @brief Get the local rotation of this node.
		/// @return Local rotation angle.
		[[nodiscard]] auto get_rotation() const -> float
		{
			return rotation_;
		}

		/// @brief Create and attach a child node of type `T`.
		///
		/// The child is owned by this node (via the underlying `Object` hierarchy).
		/// The child's constructor must accept `(Engine&, Args...)`.
		///
		/// @tparam T Node type (must derive from Node).
		/// @tparam Args Constructor argument types for T.
		/// @param args Arguments forwarded to the child's constructor.
		/// @return Reference to the created child node.
		template <NodeType T, typename... Args>
		[[nodiscard]] auto create_node(Args&&... args) -> T&
		{
			auto child = std::make_unique<T>(engine(), std::forward<Args>(args)...);
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

		/// @brief Get a mutable list of direct child nodes.
		///
		/// This list typically mirrors the node children stored in the underlying
		/// `Object` hierarchy, but exposed as `Node*` for convenience.
		///
		/// @return Mutable list of pointers to child nodes.
		[[nodiscard]] auto nodes() -> std::vector<Node*>&
		{
			return nodes_;
		}

		/// @brief Get the local transform matrix.
		///
		/// The transform is derived from local position/scale/rotation.
		///
		/// @return Local transform matrix.
		[[nodiscard]] auto transform() const -> Mat4
		{
			return transform_;
		}

		/// @brief Get the global transform matrix.
		///
		/// The global transform composes this node's local transform with all
		/// ancestor transforms up to the root.
		///
		/// @return Global/world transform matrix.
		[[nodiscard]] auto transform_global() const -> Mat4
		{
			if (parent() != nullptr)
			{
				return parent_node_->transform_global() * transform();
			}

			return transform();
		}

		/// @brief Draw this node using the provided renderer.
		///
		/// Typically emits `on_draw` for this node and may be used as the entry
		/// point for drawing subtrees depending on implementation.
		///
		/// @param x Renderer used for issuing draw calls.
		auto draw(Renderer& x) const -> void
		{
			rlPushMatrix();
			rlMultMatrixf(transform_.data());

			on_draw_(x);

			for (auto* node : nodes_)
			{
				node->draw(x);
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
		/// @brief Hide Object::create_child to only allow Node creation APIs.
		using Object::create_child;

		/// @brief Recompute cached local transform from position/scale/rotation.
		auto update_transform() -> void
		{
			auto translation = Mat4::translate({position_.x, position_.y, 0.0F});
			auto rotation = Mat4::rotate(druid::math::radians(rotation_), {1.0F, 0.0F, 0.0F});
			auto scaling = Mat4::scale({scale_.x, scale_.y, 1.0F});

			transform_ = translation * rotation * scaling;
		}

		std::vector<Node*> nodes_;
		Node* parent_node_{nullptr};
		Signal<void(Renderer&)> on_draw_;
		Mat4 transform_{Mat4::identity()};
		Vec2 position_{DefaultPosition};
		Vec2 scale_{DefaultScale};
		float rotation_{DefaultRotation};
	};
}
