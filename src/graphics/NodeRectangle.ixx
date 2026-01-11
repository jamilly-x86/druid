module;

#include <functional>
#include <glm/glm.hpp>

export module druid.graphics.NodeRectangle;

import druid.core.Engine;
import druid.graphics.Color;
import druid.graphics.Node;

export namespace druid::graphics
{
	/// @class NodeRectangle
	/// @brief Scene-graph node that renders a filled axis-aligned rectangle.
	///
	/// `NodeRectangle` is a concrete `Node` that draws a rectangle using the
	/// engine's `Renderer`. The rectangle is defined by a size vector and is
	/// rendered relative to the node's local origin.
	///
	/// The rectangle's transform (position, scale, rotation) is inherited from
	/// `Node` and applied before rendering.
	class NodeRectangle : public Node
	{
	public:
		/// @brief Construct a rectangle node associated with the given engine.
		///
		/// Registers an internal draw callback that renders a rectangle using the
		/// current size and color.
		///
		/// @param x Owning engine instance.
		explicit NodeRectangle(druid::core::Engine& x) : Node{x}
		{
			on_draw(
				[this](auto& renderer)
				{
					const auto tl = top_left();
					renderer.draw_rectangle(tl.x, tl.y, size_.x, size_.y, color_);
				});
		}

		/// @brief Virtual destructor.
		~NodeRectangle() override = default;

		NodeRectangle(const NodeRectangle&) = delete;
		NodeRectangle(NodeRectangle&&) noexcept = delete;
		auto operator=(const NodeRectangle&) -> NodeRectangle& = delete;
		auto operator=(NodeRectangle&&) noexcept -> NodeRectangle& = delete;

		/// @brief Set the rectangle size.
		///
		/// The size represents the width and height of the rectangle in local
		/// space before any node transform is applied.
		///
		/// @param x Size of the rectangle (width, height).
		auto set_size(glm::vec2 x) -> void
		{
			size_ = x;
		}

		/// @brief Get the rectangle size.
		/// @return Size of the rectangle (width, height).
		[[nodiscard]] auto get_size() const -> glm::vec2
		{
			return size_;
		}

		/// @brief Set the rectangle fill color.
		/// @param color Color used when rendering the rectangle.
		auto set_color(const Color& color) -> void
		{
			color_ = color;
		}

		/// @brief Get the rectangle fill color.
		/// @return Color used to render the rectangle.
		[[nodiscard]] auto get_color() const -> Color
		{
			return color_;
		}

		/// @brief Get the top-left corner of the rectangle in local space.
		///
		/// The rectangle is centered about the node's local origin, so the
		/// top-left corner is computed as (-width/2, -height/2).
		///
		/// @return Local-space coordinates of the top-left corner.
		[[nodiscard]] auto top_left() const -> glm::vec2
		{
			return {-size_.x * Half, -size_.y * Half};
		}

		/// @brief Get the bottom-right corner of the rectangle in local space.
		///
		/// @return Local-space coordinates of the bottom-right corner.
		[[nodiscard]] auto bottom_right() const -> glm::vec2
		{
			return size_ * Half;
		}

	private:
		static constexpr auto Half{0.5F};
		glm::vec2 size_{};
		Color color_{Color::White};
	};
}
