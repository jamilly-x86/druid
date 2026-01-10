module;

#include <raylib.h>

export module druid.graphics.renderer.Raylib;

import std;
import druid.graphics.Color;
import druid.graphics.Renderer;

export namespace druid::graphics::renderer
{
	/// @class Raylib
	/// @brief Raylib-backed implementation of the Renderer interface.
	///
	/// This class adapts the abstract `druid::graphics::Renderer` API to the
	/// Raylib graphics library. It translates engine-agnostic draw commands
	/// into Raylib function calls.
	///
	/// `Raylib` is intended to be owned and managed by higher-level services
	/// (such as `graphics::Window`) and does not manage window creation or
	/// lifetime itself.
	class Raylib : public Renderer
	{
	public:
		/// @brief Default constructor.
		Raylib() = default;

		/// @brief Virtual destructor.
		~Raylib() override = default;

		Raylib(const Raylib&) = delete;
		Raylib(Raylib&&) noexcept = delete;
		auto operator=(const Raylib&) -> Raylib& = delete;
		auto operator=(Raylib&&) noexcept -> Raylib& = delete;

		/// @brief Begin a new rendering frame.
		///
		/// Clears the background to the specified color and begins a Raylib
		/// drawing context.
		///
		/// @param clear Background clear color.
		auto begin(druid::graphics::Color clear) -> void override
		{
			const ::Color rl_color{.r = clear.red(), .g = clear.green(), .b = clear.blue(), .a = clear.alpha()};

			ClearBackground(rl_color);
			BeginDrawing();
		}

		/// @brief End the current rendering frame.
		///
		/// Finalizes the Raylib drawing context and presents the frame. A frames-
		/// per-second counter is drawn in the top-left corner for debugging.
		auto end() -> void override
		{
			DrawFPS(0, 0);
			EndDrawing();
		}

		/// @brief Draw a filled rectangle.
		///
		/// @param x X-coordinate of the rectangle origin.
		/// @param y Y-coordinate of the rectangle origin.
		/// @param width Rectangle width.
		/// @param height Rectangle height.
		/// @param color Fill color.
		auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color color) -> void override
		{
			const ::Color rl_color{.r = color.red(), .g = color.green(), .b = color.blue(), .a = color.alpha()};

			const Rectangle rect{.x = x, .y = y, .width = width, .height = height};

			DrawRectangleRec(rect, rl_color);
		}

		/// @brief Draw a text string.
		///
		/// @param x X-coordinate of the text origin.
		/// @param y Y-coordinate of the text origin.
		/// @param text UTF-8 text string to render.
		/// @param font_size Font size in pixels.
		/// @param color Text color.
		auto draw_text(float x, float y, const std::string& text, int font_size, druid::graphics::Color color) -> void override
		{
			const ::Color rl_color{.r = color.red(), .g = color.green(), .b = color.blue(), .a = color.alpha()};

			DrawText(text.c_str(), static_cast<int>(x), static_cast<int>(y), font_size, rl_color);
		}
	};
}
