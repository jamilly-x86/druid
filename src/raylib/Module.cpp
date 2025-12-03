#include <druid/raylib/Module.h>

#include <druid/raylib/Drawable.h>
#include <druid/raylib/Window.h>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cassert>
#include <variant>

import druid.core.components;

using druid::raylib::Module;

Module::Module(flecs::world& x)
{
	x.module<Module>();

	x.component<Drawable>().on_add([](flecs::entity e, Drawable&) { e.add<druid::core::Transform>(); });
	x.component<Window>();

	x.system<Window, druid::core::Size>()
		.kind(flecs::OnStart)
		.each(
			[](const Window& window, const druid::core::Size& size)
			{
				SetConfigFlags(FLAG_WINDOW_RESIZABLE);
				SetConfigFlags(FLAG_MSAA_4X_HINT);
				InitWindow(static_cast<int>(size.width), static_cast<int>(size.height), window.title.c_str());
			});

	x.system<Window, druid::core::Size>()
		.kind(flecs::PreUpdate)
		.each(
			[](const Window&, druid::core::Size& size)
			{
				if (IsWindowResized())
				{
					size.width = static_cast<float>(GetScreenWidth());
					size.height = static_cast<float>(GetScreenHeight());
				}
			});

	// x.system<Window>().kind(flecs::OnDelete).each([](Window&) {
	//	CloseWindow();
	// });

	auto pre_render = x.entity("pre_render").add(flecs::Phase).depends_on(flecs::PostUpdate);
	auto render = x.entity("render").add(flecs::Phase).depends_on(pre_render);
	auto post_render = x.entity("post_render").add(flecs::Phase).depends_on(render);

	x.system<>().kind(pre_render).run([](flecs::iter&) { ClearBackground({Black.r, Black.g, Black.b, Black.a}); });

	x.system<Drawable, druid::core::Transform>().kind(render).each(
		[](const Drawable& drawable, const druid::core::Transform& tx)
		{
			std::visit(
				[&tx](auto&& arg)
				{
					using T = std::decay_t<decltype(arg)>;

					if constexpr (std::is_same_v<T, Rectangle>)
					{
						DrawRectangle(arg.x, arg.y, arg.width, arg.height, {arg.color.r, arg.color.g, arg.color.b, arg.color.a});
					}
					else if constexpr (std::is_same_v<T, Arc>)
					{
						const auto radius_inner = arg.radius - arg.line_width / 2.0F;
						const auto radius_outer = arg.radius + arg.line_width / 2.0F;

						DrawRing({tx.position.x, tx.position.y}, radius_inner, radius_outer, arg.angle_start, arg.angle_end, arg.segments,
								 {arg.color.r, arg.color.g, arg.color.b, arg.color.a});
					}
					else if constexpr (std::is_same_v<T, TriangleStrip>)
					{
						// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
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
