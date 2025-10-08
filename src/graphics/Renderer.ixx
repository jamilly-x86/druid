module;

#include <string>

export module druid.graphics.renderer;
export import druid.graphics.color;

namespace druid::graphics
{
	export class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		auto operator=(const Renderer&) -> Renderer& = delete;
		auto operator=(Renderer&&) noexcept -> Renderer& = delete;

		virtual auto begin(druid::graphics::Color) -> void = 0;
		virtual auto end() -> void = 0;
		virtual auto draw_rectangle(float x, float y, float width, float height, druid::graphics::Color) -> void = 0;
		virtual auto draw_text(float x, float y, const std::string& text, int font_size, druid::graphics::Color color) -> void = 0;
	};
}