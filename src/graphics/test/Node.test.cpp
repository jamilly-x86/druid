#include <gtest/gtest.h>
#include <glm/glm.hpp>

import druid.core.Engine;
import druid.graphics.Node;

using druid::core::Engine;
using druid::graphics::Node;

TEST(Node, default_constructor)
{
	Engine engine;
	auto node = std::make_unique<Node>(engine);

	EXPECT_EQ(node->get_position(), Node::DefaultPosition);
	EXPECT_EQ(node->get_scale(), Node::DefaultScale);
	EXPECT_EQ(node->get_rotation(), Node::DefaultRotation);
	EXPECT_TRUE(std::empty(node->children()));
}

TEST(Node, create_node)
{
	Engine engine;
	auto root = std::make_unique<Node>(engine);
	auto& node1 = root->create_node();
	auto& node2 = root->create_node();

	EXPECT_EQ(node1.parent(), root.get());
	EXPECT_EQ(node2.parent(), root.get());
	EXPECT_EQ(std::size(root->nodes()), std::size(root->children()));
	EXPECT_EQ(std::size(root->children()), 2U);
}

TEST(Node, remove_node)
{
	Engine engine;
	auto root = std::make_unique<Node>(engine);
	auto& one = root->create_node();
	auto& two = root->create_node();
	auto& three = root->create_node();

	EXPECT_EQ(std::size(root->children()), 3U);
	EXPECT_EQ(one.parent(), root.get());
	EXPECT_EQ(two.parent(), root.get());
	EXPECT_EQ(three.parent(), root.get());

	auto node = two.remove();
	ASSERT_NE(node, nullptr);
	EXPECT_EQ(std::size(root->children()), 2U);
	EXPECT_EQ(node.get(), &two);
	EXPECT_EQ(two.parent(), nullptr);

	// Ensure removing the same node twice yields no new node->
	node = two.remove();
	EXPECT_EQ(node, nullptr);
}

TEST(Node, get_position_global)
{
	Engine engine;
	auto root = std::make_unique<Node>(engine);
	Node& child1 = root->create_node();
	Node& child2 = child1.create_node();
	Node& child3 = child2.create_node();

	// Set each node's position to (10, 10)
	constexpr auto val{10.0F};
	root->set_position({val, val});
	child1.set_position({val, val});
	child2.set_position({val, val});
	child3.set_position({val, val});

	constexpr auto test = val * 4;

	// The global position of child3 should be the sum of all positions: (40, 40)
	const auto global_pos = child3.get_position_global();

	// NOLINTBEGIN
	EXPECT_FLOAT_EQ(global_pos.x, test);
	EXPECT_FLOAT_EQ(global_pos.y, test);
	// NOLINTEND
}

TEST(Node, set_and_get_scale)
{
	Engine engine;
	auto node = std::make_unique<Node>(engine);
	const auto test = glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::min());
	node->set_scale(test);

	// NOLINTBEGIN
	EXPECT_EQ(node->get_scale().x, test.x);
	EXPECT_EQ(node->get_scale().y, test.y);
	// NOLINTEND
}

TEST(Node, set_and_get_rotation)
{
	Engine engine;
	auto node = std::make_unique<Node>(engine);

	constexpr auto test{45.0F};
	node->set_rotation(test);
	EXPECT_EQ(node->get_rotation(), test);
}
