module;

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
		auto operator=(const NodeRectangle&) -> NodeRectangle& = delete;
		NodeRectangle(NodeRectangle&&) noexcept = delete;
		auto operator=(NodeRectangle&&) noexcept -> NodeRectangle& = delete;

		auto set_size(float width, float height) -> void
		{
			width_ = width;
			height_ = height;
		}

		auto get_size() const -> glm::vec2
		{
			return {width_, height_};
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
			return {-width_ * 0.5F, -height_ * 0.5F};
		}

		auto bottom_right() const -> glm::vec2
		{
			return {width_, height_};
		}

	private:
		float width_{};
		float height_{};
		Color color_{Color::White};
	};
}