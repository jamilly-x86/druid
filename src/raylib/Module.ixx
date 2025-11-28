module;

#include <flecs.h>
#include <raylib.h>

export module druid.raylib;
export import druid.raylib.rectangle;
export import druid.raylib.window;

namespace druid::raylib
{
	export struct Module
	{
		Module(flecs::world& x)
		{
			x.module<Module>();

			x.component<Rectangle>();
			x.component<Window>();

			x.system<Window>().kind(flecs::OnStart).each([](Window& window) { InitWindow(window.width, window.height, window.title.c_str()); });
			x.system<Window>().kind(flecs::OnDelete).each([](auto, auto&) { CloseWindow(); });

			x.system<>()
				.kind(flecs::OnUpdate)
				.run(
					[](flecs::iter& it)
					{
						ClearBackground(BLACK);
						BeginDrawing();

						DrawFPS(0, 0);
						EndDrawing();

						if (WindowShouldClose())
						{
							it.world().quit();
						}
					});

			x.system<Rectangle>().each([](const Rectangle& x) { DrawRectangle(x.x, x.y, x.width, x.height, x.color); });
		}
	};
}