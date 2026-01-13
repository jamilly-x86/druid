#include <gtest/gtest.h>
#include <glm/glm.hpp>

import druid.graphics.NodeRectangle;

using druid::graphics::NodeRectangle;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

// Test that a centered rectangle has correct local-space bounds
TEST(NodeRectangle, centered_bounds)
{
	auto rect = std::make_unique<NodeRectangle>();

	rect->set_size({100.0F, 50.0F});

	auto tl = rect->top_left();
	auto br = rect->bottom_right();

	// For a centered rectangle with size (100, 50):
	// top_left should be (-50, -25)
	// bottom_right should be (50, 25)
	EXPECT_FLOAT_EQ(tl.x, -50.0F);
	EXPECT_FLOAT_EQ(tl.y, -25.0F);
	EXPECT_FLOAT_EQ(br.x, 50.0F);
	EXPECT_FLOAT_EQ(br.y, 25.0F);
}

// Test AABB collision detection with two rectangles
TEST(NodeRectangle, collision_detection_overlapping)
{
	auto rect1 = std::make_unique<NodeRectangle>();
	auto rect2 = std::make_unique<NodeRectangle>();

	// Rectangle 1: 100x100 at (0, 0)
	// World bounds: (-50, -50) to (50, 50)
	rect1->set_size({100.0F, 100.0F});
	rect1->set_position({0.0F, 0.0F});

	// Rectangle 2: 50x50 at (70, 0) - should overlap
	// World bounds: (45, -25) to (95, 25)
	rect2->set_size({50.0F, 50.0F});
	rect2->set_position({70.0F, 0.0F}); // Calculate world-space bounds
	auto r1_tl = rect1->top_left() + rect1->get_position();
	auto r1_br = rect1->bottom_right() + rect1->get_position();
	auto r2_tl = rect2->top_left() + rect2->get_position();
	auto r2_br = rect2->bottom_right() + rect2->get_position();

	// AABB collision detection
	const bool collision = r1_tl.x < r2_br.x && r1_br.x > r2_tl.x && r1_tl.y < r2_br.y && r1_br.y > r2_tl.y;

	EXPECT_TRUE(collision);
}

// Test AABB collision detection with non-overlapping rectangles
TEST(NodeRectangle, collision_detection_not_overlapping)
{
	auto rect1 = std::make_unique<NodeRectangle>();
	auto rect2 = std::make_unique<NodeRectangle>();

	// Rectangle 1: 100x100 at (0, 0)
	rect1->set_size({100.0F, 100.0F});
	rect1->set_position({0.0F, 0.0F});

	// Rectangle 2: 50x50 at (200, 0) - should not overlap
	rect2->set_size({50.0F, 50.0F});
	rect2->set_position({200.0F, 0.0F});

	// Calculate world-space bounds
	auto r1_tl = rect1->top_left() + rect1->get_position();
	auto r1_br = rect1->bottom_right() + rect1->get_position();
	auto r2_tl = rect2->top_left() + rect2->get_position();
	auto r2_br = rect2->bottom_right() + rect2->get_position();

	// AABB collision detection
	const bool collision = r1_tl.x < r2_br.x && r1_br.x > r2_tl.x && r1_tl.y < r2_br.y && r1_br.y > r2_tl.y;

	EXPECT_FALSE(collision);
}

// Test edge-touching rectangles (should not collide)
TEST(NodeRectangle, collision_detection_edge_touching)
{
	auto rect1 = std::make_unique<NodeRectangle>();
	auto rect2 = std::make_unique<NodeRectangle>();

	// Rectangle 1: 100x100 at (0, 0)
	// World bounds: (-50, -50) to (50, 50)
	rect1->set_size({100.0F, 100.0F});
	rect1->set_position({0.0F, 0.0F});

	// Rectangle 2: 50x50 at (75, 0)
	// World bounds: (50, -25) to (100, 25)
	// Edges touch exactly at x=50
	rect2->set_size({50.0F, 50.0F});
	rect2->set_position({75.0F, 0.0F});

	auto r1_tl = rect1->top_left() + rect1->get_position();
	auto r1_br = rect1->bottom_right() + rect1->get_position();
	auto r2_tl = rect2->top_left() + rect2->get_position();
	auto r2_br = rect2->bottom_right() + rect2->get_position();

	// Verify the bounds are correct
	EXPECT_FLOAT_EQ(r1_br.x, 50.0F);
	EXPECT_FLOAT_EQ(r2_tl.x, 50.0F);

	// With < comparison, touching edges should not collide
	const bool collision = r1_tl.x < r2_br.x && r1_br.x > r2_tl.x && r1_tl.y < r2_br.y && r1_br.y > r2_tl.y;

	EXPECT_FALSE(collision);
}

// Test pong-like scenario: ball hitting paddle
TEST(NodeRectangle, pong_ball_paddle_collision)
{
	auto paddle = std::make_unique<NodeRectangle>();
	auto ball = std::make_unique<NodeRectangle>();

	// Paddle: 25x100 at (128, 360)
	paddle->set_size({25.0F, 100.0F});
	paddle->set_position({128.0F, 360.0F});

	// Ball: 24x24 approaching paddle from the left at (115, 360)
	ball->set_size({24.0F, 24.0F});
	ball->set_position({115.0F, 360.0F});

	auto paddle_tl = paddle->top_left() + paddle->get_position();
	auto paddle_br = paddle->bottom_right() + paddle->get_position();
	auto ball_tl = ball->top_left() + ball->get_position();
	auto ball_br = ball->bottom_right() + ball->get_position();

	// Check bounds
	// Paddle: (128 - 12.5, 360 - 50) to (128 + 12.5, 360 + 50) = (115.5, 310) to (140.5, 410)
	// Ball: (115 - 12, 360 - 12) to (115 + 12, 360 + 12) = (103, 348) to (127, 372)
	EXPECT_FLOAT_EQ(paddle_tl.x, 115.5F);
	EXPECT_FLOAT_EQ(paddle_br.x, 140.5F);
	EXPECT_FLOAT_EQ(ball_tl.x, 103.0F);
	EXPECT_FLOAT_EQ(ball_br.x, 127.0F);

	// Ball right edge (127) > paddle left edge (115.5) - should collide
	const bool collision = ball_tl.x < paddle_br.x && ball_br.x > paddle_tl.x && ball_tl.y < paddle_br.y && ball_br.y > paddle_tl.y;

	EXPECT_TRUE(collision);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
