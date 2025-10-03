module;

#include <raylib.h>

export module druid.graphics.renderer.raylib;
import druid.graphics.renderer;

namespace druid::graphics::renderer
{
	export class Raylib : public Renderer
	{
	public:
		Raylib() = default;
		~Raylib() override = default;
		Raylib(const Raylib&) = delete;
		Raylib(Raylib&&) noexcept = delete;
		auto operator=(const Raylib&) -> Raylib& = delete;
		auto operator=(Raylib&&) noexcept -> Raylib& = delete;

		auto begin(druid::graphics::Color clear) -> void override
		{
			const ::Color rl_color{.r = clear.red(), .g = clear.green(), .b = clear.blue(), .a = clear.alpha()};
			ClearBackground(rl_color);
			BeginDrawing();
		}

		auto end() -> void override
		{
			EndDrawing();
		}

		auto draw_rectangle(int x, int y, int width, int height, druid::graphics::Color color) -> void override
		{
			const ::Color rl_color{.r = color.red(), .g = color.green(), .b = color.blue(), .a = color.alpha()};
			DrawRectangle(x, y, width, height, rl_color);
		}
	};
}