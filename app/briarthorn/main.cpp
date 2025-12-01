#include <druid/raylib/Module.h>
#include <flecs.h>
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include "druid/raylib/Drawable.h"
#include "druid/raylib/Window.h"

auto main() -> int
{
	// NOLINTBEGIN
	flecs::world world;

	// clang-format off
	world.import<druid::raylib::Module>();
	// clang-format on

	auto window = world.entity();
	window.set<druid::raylib::Window>({.title = "test", .width = 1280, .height = 720});

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
		world.entity().set<druid::raylib::Drawable>(r);
	}

	world.entity().set<druid::raylib::Drawable>(druid::raylib::TriangleStrip::from_arc(
		{.x = 640, .y = 360, .radius = 100, .angle_start = -45, .angle_end = 300, .line_width = 4, .segments = 120, .color = {255, 255, 0, 255}}));

	world.entity().set<druid::raylib::Drawable>(druid::raylib::TriangleStrip::from_arc(
		{.x = 640, .y = 360, .radius = 250, .angle_start = -50, .angle_end = 300, .line_width = 4, .segments = 305, .color = {255, 255, 0, 255}}));

	// Run indefinitely.
	while (world.progress())
	{
	}

	// NOLINTEND

	return EXIT_SUCCESS;
}