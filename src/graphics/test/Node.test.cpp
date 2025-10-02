#include <gtest/gtest.h>
#include <ranges>

import druid.graphics.node;

using druid::graphics::Node;

TEST(Node, default_constructor)
{
	Node node;
	EXPECT_EQ(node.get_position(), Node::DefaultPosition);
	EXPECT_EQ(node.get_scale(), Node::DefaultScale);
	EXPECT_EQ(node.get_rotation(), Node::DefaultRotation);
	EXPECT_TRUE(std::empty(node.children()));
}

TEST(Node, create_node)
{
	Node root;
	(void)root.create_node();
	(void)root.create_node();

	EXPECT_EQ(std::size(root.children()), 2U);
}

TEST(Node, remove_node)
{
	Node root;
	auto& one = root.create_node();
	auto& two = root.create_node();
	auto& three = root.create_node();

	EXPECT_EQ(std::size(root.children()), 3U);
	EXPECT_EQ(one.parent(), &root);
	EXPECT_EQ(two.parent(), &root);
	EXPECT_EQ(three.parent(), &root);

	auto node = two.remove();
	ASSERT_NE(node, nullptr);
	EXPECT_EQ(std::size(root.children()), 2U);
	EXPECT_EQ(node.get(), &two);
	EXPECT_EQ(two.parent(), nullptr);

	// Ensure removing the same node twice yields no new node.
	node = two.remove();
	EXPECT_EQ(node, nullptr);
}

TEST(Node, get_position_global)
{
	Node root;
	Node& child1 = root.create_node();
	Node& child2 = child1.create_node();
	Node& child3 = child2.create_node();

	// Set each node's position to (10, 10)
	root.set_position({10.0f, 10.0f});
	child1.set_position({10.0f, 10.0f});
	child2.set_position({10.0f, 10.0f});
	child3.set_position({10.0f, 10.0f});

	// The global position of child3 should be the sum of all positions: (40, 40)
	const auto global_pos = child3.get_position_global();

	EXPECT_FLOAT_EQ(global_pos.x, 40.0f);
	EXPECT_FLOAT_EQ(global_pos.y, 40.0f);
}

TEST(Node, set_and_get_scale)
{
	Node node;
	node.set_scale({2.0f, 3.0f});
	EXPECT_EQ(node.get_scale().x, 2.0f);
	EXPECT_EQ(node.get_scale().y, 3.0f);
}

TEST(Node, set_and_get_rotation)
{
	Node node;
	node.set_rotation(45.0f);
	EXPECT_EQ(node.get_rotation(), 45.0f);
}
