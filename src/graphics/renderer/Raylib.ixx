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
		Raylib(const Raylib&) = delete;
		Raylib(Raylib&&) noexcept = delete;
		auto operator=(const Raylib&) -> Raylib& = delete;
		auto operator=(Raylib&&) noexcept -> Raylib& = delete;

		auto begin(Color clear) -> void override
		{
			::Color rl_color{clear.red(), clear.green(), clear.blue(), clear.alpha()};
			ClearBackground(rl_color);
			BeginDrawing();
		}

		auto end() -> void override
		{
			EndDrawing();
		}

		auto draw_rectangle(int x, int y, int width, int height, Color color) -> void override
		{
			::Color rl_color{color.red(), color.green(), color.blue(), color.alpha()};
			DrawRectangle(x, y, width, height, rl_color);
		}
	};
}