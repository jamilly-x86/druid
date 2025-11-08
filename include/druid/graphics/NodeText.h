#pragma once

#include <druid/graphics/Color.h>
#include <druid/graphics/Node.h>
#include <string>
#include <string_view>

namespace druid::graphics
{
	class NodeText : public Node
	{
	public:
		static constexpr auto DefaultFontSize{20};

		NodeText(druid::core::Engine& x) : Node{x}
		{
			on_draw([this](auto& renderer) { renderer.draw_text(0, 0, text_, font_size_, color_); });
		}

		~NodeText() override = default;
		NodeText(const NodeText&) = delete;
		NodeText(NodeText&&) noexcept = delete;
		auto operator=(const NodeText&) -> NodeText& = delete;
		auto operator=(NodeText&&) noexcept -> NodeText& = delete;

		auto set_text(std::string_view x) -> void
		{
			text_ = x;
		}

		[[nodiscard]] auto get_text() const -> std::string_view
		{
			return text_;
		}

		auto set_font_size(int x) -> void
		{
			font_size_ = x;
		}

		[[nodiscard]] auto get_font_size() const -> int
		{
			return font_size_;
		}

		auto set_color(const Color& color) -> void
		{
			color_ = color;
		}

		[[nodiscard]] auto get_color() const -> Color
		{
			return color_;
		}

	private:
		std::string text_;
		int font_size_{DefaultFontSize};
		Color color_{Color::White};
	};
}