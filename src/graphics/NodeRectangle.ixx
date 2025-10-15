module;

#include <core/Signal.h>
#include <glm/vec2.hpp>

export module druid.graphics.node.rectangle;

import druid.graphics.node;
import druid.graphics.color;

namespace druid::graphics
{
	export class NodeRectangle : public Node
	{
	public:
		NodeRectangle()
		{
			on_draw(
				[this](auto& renderer)
				{
					const auto tl = top_left();
					const auto br = bottom_right();
					renderer.draw_rectangle(tl.x, tl.y, br.x, br.y, color_);
				});
		}

		~NodeRectangle() override = default;
		NodeRectangle(const NodeRectangle&) = delete;
		NodeRectangle(NodeRectangle&&) noexcept = delete;
		auto operator=(const NodeRectangle&) -> NodeRectangle& = delete;
		auto operator=(NodeRectangle&&) noexcept -> NodeRectangle& = delete;

		auto set_size(glm::vec2 x) -> void
		{
			size_ = x;
		}

		auto get_size() const -> glm::vec2
		{
			return size_;
		}

		auto set_color(const Color& color) -> void
		{
			color_ = color;
		}

		auto get_color() const -> Color
		{
			return color_;
		}

		auto top_left() const -> glm::vec2
		{
			return {-size_.x * Half, -size_.y * Half};
		}

		auto bottom_right() const -> glm::vec2
		{
			return size_;
		}

	private:
		static constexpr auto Half{0.5};
		glm::vec2 size_{};
		Color color_{Color::White};
	};
}