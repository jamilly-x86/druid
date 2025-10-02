#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include <raylib.h>
#include <rlgl.h>

import druid.graphics.node;

auto main() -> int
try
{
	// NOLINTBEGIN

	constexpr auto width{1280};
	constexpr auto height{720};

	InitWindow(width, height, "Hello Raylib with C++23");

	druid::graphics::Node root;

	auto& paddle1 = root.create_node();
	paddle1.set_position({width * 0.1, height * 0.5});
	paddle1.on_draw([] { DrawRectangle(0, 0, 25, 100, GREEN); });

	auto& paddle2 = root.create_node();
	paddle2.set_position({width * 0.9, height * 0.5});
	paddle2.on_draw([] { DrawRectangle(0, 0, 25, 100, GREEN); });

	auto& ball = root.create_node();
	ball.set_position({width * 0.5, height * 0.5});
	ball.on_draw([] { DrawRectangle(-12, -12, 24, 24, RAYWHITE); });

	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		root.draw();
		DrawText("DRUID PONG (C++26)", static_cast<int>(width * 0.4), 0, 24, LIGHTGRAY);

		DrawFPS(0, 0);
		EndDrawing();
	}
	CloseWindow();

	// NOLINTEND

	return EXIT_SUCCESS;
}
catch(const std::exception& e)
{
	std::cerr << "Exception: " << e.what() << "\n";
	return EXIT_FAILURE;
}