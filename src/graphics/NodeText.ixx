module;

#include <functional>
#include <string>
#include <string_view>

export module druid.graphics.NodeText;

import druid.graphics.Color;
import druid.graphics.Node;

export namespace druid::graphics
{
	/// @class NodeText
	/// @brief Scene-graph node that renders 2D text.
	///
	/// `NodeText` is a specialized `Node` that draws a text string using the
	/// engine's `Renderer`. Rendering is performed via an internal `on_draw`
	/// callback that issues a text draw call at the node's local origin.
	///
	/// The node's transform (position, scale, rotation) is inherited from `Node`
	/// and affects where the text is rendered in the scene.
	class NodeText : public Node
	{
	public:
		/// @brief Default font size used when none is specified.
		static constexpr auto DefaultFontSize{20};

		/// @brief Construct a text node.
		///
		/// Registers an internal draw callback that renders the stored text
		/// using the current font size and color.
		NodeText()
		{
			on_draw(
				[this](auto& renderer)
				{
					const auto pos = get_position_global();
					renderer.draw_text(pos.x, pos.y, text_, font_size_, color_);
				});
		}

		/// @brief Virtual destructor.
		~NodeText() override = default;

		NodeText(const NodeText&) = delete;
		NodeText(NodeText&&) noexcept = delete;
		auto operator=(const NodeText&) -> NodeText& = delete;
		auto operator=(NodeText&&) noexcept -> NodeText& = delete;

		/// @brief Set the text string to render.
		/// @param x UTF-8 text string to display.
		auto set_text(std::string_view x) -> void
		{
			text_ = x;
		}

		/// @brief Get the current text string.
		/// @return View of the text currently stored in the node.
		[[nodiscard]] auto get_text() const -> std::string_view
		{
			return text_;
		}

		/// @brief Set the font size used for rendering the text.
		/// @param x Font size in pixels (or renderer-defined units).
		auto set_font_size(int x) -> void
		{
			font_size_ = x;
		}

		/// @brief Get the current font size.
		/// @return Font size in pixels (or renderer-defined units).
		[[nodiscard]] auto get_font_size() const -> int
		{
			return font_size_;
		}

		/// @brief Set the text color.
		/// @param color Color used when rendering the text.
		auto set_color(const Color& color) -> void
		{
			color_ = color;
		}

		/// @brief Get the current text color.
		/// @return Color used to render the text.
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
