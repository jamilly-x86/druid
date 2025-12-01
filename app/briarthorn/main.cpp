#include <flecs.h>
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include "druid/raylib/Drawable.h"
#include "druid/raylib/Window.h"
#include <druid/raylib/Module.h>


auto main() -> int
{
	flecs::world world;

	// clang-format off
	auto module = world.import<druid::raylib::Module>();
	// clang-format on

	auto window = world.entity();
	window.set<druid::raylib::Window>({.title = "test", .width = 1280, .height = 720});

	for (auto i = 0; i < 50; i++)
	{
		const auto angle = GetRandomValue(0, 360);
		const auto radius = GetRandomValue(0, 360);

		druid::raylib::Rectangle r{};
		r.x = static_cast<int>(640 + radius * std::cos(angle * DEG2RAD));
		r.y = static_cast<int>(360 + radius * std::sin(angle * DEG2RAD));
		r.width = 25;
		r.height = 25;
		r.color = {125, 125, 125, 255};
		world.entity().set<druid::raylib::Drawable>(r);
	}

	world.entity().set<druid::raylib::Drawable>(druid::raylib::TriangleStrip::make_arc(640, 360, 100, -45, 300, 4, 120, { 255, 255, 0, 255 }));
	world.entity().set<druid::raylib::Drawable>(druid::raylib::TriangleStrip::make_arc(640, 360, 250, -50, 305, 4, 120, {255, 255, 0, 255}));

	// Run indefinitely.
	while (world.progress())
	{
	}

	return EXIT_SUCCESS;
}