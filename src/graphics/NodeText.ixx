module;

#include <string>

export module druid.graphics.node.text;

import druid.graphics.node;
import druid.graphics.color;

namespace druid::graphics
{
	export class NodeText : public Node
	{
	public:
		NodeText()
		{
			on_draw(
				[this](auto& renderer)
				{
					renderer.draw_text(0, 0, text, font_size_, color_);
				});
		}

		~NodeText() override = default;
		NodeText(const NodeText&) = delete;
		NodeText(NodeText&&) noexcept = delete;
		auto operator=(const NodeText&) -> NodeText& = delete;
		auto operator=(NodeText&&) noexcept -> NodeText& = delete;

		auto set_text(std::string_view x) -> void
		{
			text = x;
		}

		[[nodiscard]] auto get_text() const -> std::string_view
		{
			return text;
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

		auto get_color() const -> Color
		{
			return color_;
		}

	private:
		std::string text;
		int font_size_{20};
		Color color_{Color::White};
	};
}