#include <druid/anchor/Components.h>
#include <druid/anchor/Module.h>
#include <druid/raylib/Module.h>
#include <flecs.h>
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include "druid/raylib/Drawable.h"
#include "druid/raylib/Window.h"

using druid::anchor::Anchor;
using druid::raylib::Arc;
using druid::raylib::Drawable;

auto main() -> int
{
	// NOLINTBEGIN
	flecs::world world;

	// clang-format off
	world.import<druid::raylib::Module>();
	world.import<druid::anchor::Module>();
	// clang-format on

	auto window = world.entity();
	window.set<druid::raylib::Window>({.title = "test"});
	window.set<druid::core::Size>({.width = 1280, .height = 720});

	for (auto i = 0; i < 50; i++)
	{
		const auto angle = GetRandomValue(0, 360);
		const auto radius = GetRandomValue(0, 360);

		druid::raylib::Rectangle r{};
		r.x = static_cast<int>(640 + (radius * std::cos(angle * DEG2RAD)));
		r.y = static_cast<int>(360 + (radius * std::sin(angle * DEG2RAD)));
		r.width = 25;
		r.height = 25;
		r.color = {125, 125, 125, 255};
		world.entity().set<Drawable>(r);
	}

	auto arc_inner = world.entity();
	arc_inner.set<Drawable>(Arc{.radius = 100, .angle_start = -45, .angle_end = 300, .line_width = 4, .segments = 120, .color = {255, 255, 0, 255}});
	arc_inner.set<Anchor>({.point = Anchor::Point::Center, .target = Anchor::Center(window)});

	auto arc_outer = world.entity();
	arc_outer.set<Drawable>(Arc{.radius = 250, .angle_start = -50, .angle_end = 300, .line_width = 4, .segments = 305, .color = {255, 255, 0, 255}});
	arc_outer.set<Anchor>({.point = Anchor::Point::Center, .target = Anchor::Center(window)});

	// Run indefinitely.
	while (world.progress())
	{
	}

	// NOLINTEND

	return EXIT_SUCCESS;
}