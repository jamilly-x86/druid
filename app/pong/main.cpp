#include <chrono>
#include <exception>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <magic_enum/magic_enum.hpp>

#include <druid/core/Engine.h>
#include <druid/core/Event.h>
#include <druid/graphics/Color.h>
import druid.graphics.Node;
#include <druid/graphics/NodeRectangle.h>
#include <druid/graphics/NodeText.h>
#include <druid/graphics/Window.h>

auto main() -> int
try
{
	druid::core::Engine engine;
	auto& window = engine.create_service<druid::graphics::Window>();

	// NOLINTBEGIN

	constexpr auto width{1280};
	constexpr auto height{720};

	auto& root = window.root_node();

	auto& title = root.create_node<druid::graphics::NodeText>();
	title.set_position({width * 0.42F, 0.0F});
	title.set_text("DRUID PONG (C++26)");
	title.set_font_size(24);
	title.set_color(druid::graphics::Color::White);

	auto& scoreLeft = root.create_node<druid::graphics::NodeText>();
	scoreLeft.set_position({width * 0.3F, height * 0.1F});
	scoreLeft.set_text("0");
	scoreLeft.set_font_size(40);
	scoreLeft.set_color(druid::graphics::Color::White);

	auto& scoreRight = root.create_node<druid::graphics::NodeText>();
	scoreRight.set_position({width * 0.7F, height * 0.1F});
	scoreRight.set_text("0");
	scoreRight.set_font_size(40);
	scoreRight.set_color(druid::graphics::Color::White);

	auto& paddle1 = root.create_node<druid::graphics::NodeRectangle>();
	paddle1.set_position({width * 0.1, height * 0.5});
	paddle1.set_size({25, 100});
	paddle1.set_color(druid::graphics::Color::Green);

	auto& paddle2 = root.create_node<druid::graphics::NodeRectangle>();
	paddle2.set_position({width * 0.9, height * 0.5});
	paddle2.set_size({25, 100});
	paddle2.set_color(druid::graphics::Color::Red);

	auto& ball = root.create_node<druid::graphics::NodeRectangle>();
	ball.set_position({width * 0.5, height * 0.5});
	ball.set_size({24, 24});
	ball.set_color(druid::graphics::Color::White);

	engine.on_event_window([&engine](auto) { engine.quit(); });

	auto paddle1_move_up = false;
	auto paddle1_move_down = false;
	engine.on_event_keyboard(
		[&paddle1_move_up, &paddle1_move_down](auto& x)
		{
			std::cout << "State: " << magic_enum::enum_name(x.type) << " Key: " << magic_enum::enum_name(x.key) << "\n";

			using druid::core::EventKeyboard;
			using Key = EventKeyboard::Key;

			paddle1_move_up = x.type == EventKeyboard::Type::KeyPressed and x.key == Key::Up;
			paddle1_move_down = x.type == EventKeyboard::Type::KeyPressed and x.key == Key::Down;
		});

	engine.on_update_fixed(
		[&paddle1, &paddle2, &ball, &paddle1_move_up, &paddle1_move_down](std::chrono::steady_clock::duration dt)
		{
			auto velocity_ball = glm::vec2{50.0F, 0.0F};
			auto velocity_paddle = glm::vec2{0.0F, 150.0F};

			// Move Ball
			using seconds = std::chrono::duration<float>;
			const auto dt_seconds = std::chrono::duration_cast<seconds>(dt).count();
			auto position = glm::vec2{velocity_ball.x * dt_seconds, velocity_ball.y * dt_seconds};
			ball.set_position(position + ball.get_position());

			// Move Paddle
			velocity_paddle.y *= paddle1_move_up ? -1 : paddle1_move_down ? 1 : 0;
			position = velocity_paddle * dt_seconds;
			paddle1.set_position(paddle1.get_position() + position);

			const auto ball_top_left = ball.top_left() + ball.get_position();
			const auto ball_bottom_right = ball.bottom_right() + ball.get_position();
			const auto paddle1_top_left = paddle1.top_left() + paddle1.get_position();
			const auto paddle1_bottom_right = paddle1.bottom_right() + paddle1.get_position();
			const auto paddle2_top_left = paddle2.top_left() + paddle2.get_position();
			const auto paddle2_bottom_right = paddle2.bottom_right() + paddle2.get_position();

			// Check Collisions with window borders
			if (ball_top_left.x < 0 || ball_bottom_right.x > width)
			{
				velocity_ball.x = -velocity_ball.x;
			}

			if (ball_top_left.y < 0 || ball_bottom_right.y > height)
			{
				velocity_ball.y = -velocity_ball.y;
			}

			// Check Collisions with paddles
			if (ball_top_left.x < paddle1_bottom_right.x && ball_bottom_right.y > paddle1_top_left.y && ball_top_left.y < paddle1_bottom_right.y)
			{
				velocity_ball.x = -velocity_ball.x;
			}

			if (ball_bottom_right.x > paddle2_top_left.x && ball_bottom_right.y > paddle2_top_left.y && ball_top_left.y < paddle2_bottom_right.y)
			{
				velocity_ball.x = -velocity_ball.x;
			}
		});

	// NOLINTEND

	return engine.run();
}
catch (const std::exception& e)
{
	std::cerr << "Exception: " << e.what() << "\n";
	return EXIT_FAILURE;
}