#pragma once

#include <druid/graphics/Color.h>
#include <string>

namespace druid::graphics
{
	/// @class Renderer
	/// @brief Abstract interface for issuing graphics draw commands.
	///
	/// `Renderer` defines a minimal, backend-agnostic API for rendering 2D
	/// primitives and text. Concrete implementations (e.g., Raylib, OpenGL,
	/// Vulkan, or headless renderers) translate these commands into backend-
	/// specific draw calls.
	///
	/// The renderer follows a simple frame lifecycle:
	/// 1. `begin()` — prepare the frame and clear the background
	/// 2. Draw calls (`draw_rectangle`, `draw_text`, …)
	/// 3. `end()` — finalize and present the frame
	class Renderer
	{
	public:
		/// @brief Default constructor.
		Renderer() = default;

		/// @brief Virtual destructor for safe polymorphic deletion.
		virtual ~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		auto operator=(const Renderer&) -> Renderer& = delete;
		auto operator=(Renderer&&) noexcept -> Renderer& = delete;

		/// @brief Begin a new rendering frame.
		///
		/// Implementations should prepare the rendering context and clear the
		/// frame buffer using the provided color.
		///
		/// @param clear Background clear color for the frame.
		virtual auto begin(druid::graphics::Color clear) -> void = 0;

		/// @brief End the current rendering frame.
		///
		/// Implementations should finalize the frame and present the rendered
		/// contents to the display or output target.
		virtual auto end() -> void = 0;

		/// @brief Draw a filled axis-aligned rectangle.
		///
		/// @param x X-coordinate of the rectangle origin.
		/// @param y Y-coordinate of the rectangle origin.
		/// @param width Width of the rectangle.
		/// @param height Height of the rectangle.
		/// @param color Fill color of the rectangle.
		virtual auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color color) -> void = 0;

		/// @brief Draw a text string.
		///
		/// @param x X-coordinate of the text origin.
		/// @param y Y-coordinate of the text origin.
		/// @param text UTF-8 text string to render.
		/// @param font_size Font size in pixels (or backend-defined units).
		/// @param color Text color.
		virtual auto draw_text(float x, float y, const std::string& text, int font_size, druid::graphics::Color color) -> void = 0;
	};
}
