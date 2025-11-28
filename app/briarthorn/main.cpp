#include <flecs.h>
#include <raylib.h>
#include <cmath>
#include <cstdlib>

import druid.raylib;

auto main() -> int
{
	flecs::world world;

	// clang-format off
	world.import<druid::raylib::Module>();
	// clang-format on

	auto window = world.entity();
	window.set<druid::raylib::Window>({.title = "test", .width = 1280, .height = 720});

	for (auto i = 0; i < 1000; i++)
	{
		auto e = world.entity().add<druid::raylib::Rectangle>();
		auto &r = e.get_mut<druid::raylib::Rectangle>();

		auto angle = GetRandomValue(0, 360);
		auto radius = GetRandomValue(0, 360);

		r.x = 640 + radius * std::cos(angle * DEG2RAD);
		r.y = 360 + radius * std::sin(angle * DEG2RAD);
		r.width = 25;
		r.height = 25;
		r.color = RED;
	}

	// Run indefinitely.
	while (world.progress())
	{
	}

	return EXIT_SUCCESS;
}