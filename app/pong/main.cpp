#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>

import druid.core.engine;
import druid.graphics.node;
import druid.graphics.window;
import druid.graphics.color;

using druid::graphics::Color;

auto main() -> int
try
{
	druid::core::Engine engine;
	auto& window = engine.create_child<druid::graphics::Window>();
	// NOLINTBEGIN

	constexpr auto width{1280};
	constexpr auto height{720};

	auto& root = window.root_node();

	auto& paddle1 = root.create_node();
	paddle1.set_position({width * 0.1, height * 0.5});
	paddle1.on_draw([](auto& renderer) { renderer.draw_rectangle(0, 0, 25, 100, Color::Green); });

	auto& paddle2 = root.create_node();
	paddle2.set_position({width * 0.9, height * 0.5});
	paddle2.on_draw([](auto& renderer) { renderer.draw_rectangle(0, 0, 25, 100, Color::Red); });

	auto& ball = root.create_node();
	ball.set_position({width * 0.5, height * 0.5});
	ball.on_draw([](auto& renderer) { renderer.draw_rectangle(-12, -12, 24, 24, Color::White); });

	// NOLINTEND

	return engine.run();
}
catch (const std::exception& e)
{
	std::cerr << "Exception: " << e.what() << "\n";
	return EXIT_FAILURE;
}