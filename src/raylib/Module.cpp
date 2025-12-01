#include <druid/raylib/Drawable.h>
#include <druid/raylib/Module.h>
#include <druid/raylib/Window.h>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cassert>
#include <variant>

namespace druid::raylib
{
	Module::Module(flecs::world& x)
	{
		x.module<Module>();

		x.component<Drawable>();
		x.component<Window>();

		x.system<Window>().kind(flecs::OnStart).each([](Window& window) { InitWindow(window.width, window.height, window.title.c_str()); });

		// x.system<Window>().kind(flecs::OnDelete).each([](Window&) {
		//	CloseWindow();
		// });

		auto pre_render = x.entity("pre_render").add(flecs::Phase).depends_on(flecs::PostUpdate);
		auto render = x.entity("render").add(flecs::Phase).depends_on(pre_render);
		auto post_render = x.entity("post_render").add(flecs::Phase).depends_on(render);

		x.system<>()
			.kind(pre_render)
			.run(
				[](flecs::iter&)
				{
					BeginDrawing();
					ClearBackground({0, 0, 0, 255});
				});

		x.system<Drawable>().kind(render).each(
			[](const Drawable& drawable)
			{
				std::visit(
					[](auto&& arg)
					{
						using T = std::decay_t<decltype(arg)>;

						if constexpr (std::is_same_v<T, Rectangle>)
						{
							DrawRectangle(arg.x, arg.y, arg.width, arg.height, {arg.color.r, arg.color.g, arg.color.b, arg.color.a});
						}
						else if constexpr (std::is_same_v<T, TriangleStrip>)
						{
							DrawTriangleStrip(reinterpret_cast<const Vector2*>(arg.vertices.data()), static_cast<int>(arg.vertices.size()),
											  {arg.color.r, arg.color.g, arg.color.b, arg.color.a});
						}
					},
					drawable);
			});

		x.system<>()
			.kind(post_render)
			.run(
				[](flecs::iter& it)
				{
					DrawFPS(0, 0);
					EndDrawing();

					if (WindowShouldClose())
					{
						it.world().quit();
					}
				});
	}
}