#pragma once

#include <druid/graphics/Color.h>
#include <druid/graphics/Node.h>
#include <glm/vec2.hpp>

namespace druid::graphics
{
	class NodeRectangle : public Node
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

		[[nodiscard]] auto get_size() const -> glm::vec2
		{
			return size_;
		}

		auto set_color(const Color& color) -> void
		{
			color_ = color;
		}

		[[nodiscard]] auto get_color() const -> Color
		{
			return color_;
		}

		[[nodiscard]] auto top_left() const -> glm::vec2
		{
			return {-size_.x * Half, -size_.y * Half};
		}

		[[nodiscard]] auto bottom_right() const -> glm::vec2
		{
			return size_;
		}

	private:
		static constexpr auto Half{0.5};
		glm::vec2 size_{};
		Color color_{Color::White};
	};
}