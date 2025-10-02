#include <gtest/gtest.h>
#include <glm/vec2.hpp>

import druid.graphics.node;

using druid::graphics::Node;

TEST(Node, default_constructor)
{
	const Node node;
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
	constexpr auto val{10.0F};
	root.set_position({val, val});
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
	Node node;
	constexpr auto test{glm::vec2{2.0F, 3.0F}};
	node.set_scale(test);

	// NOLINTBEGIN
	EXPECT_EQ(node.get_scale().x, test.x);
	EXPECT_EQ(node.get_scale().y, test.y);
	// NOLINTEND
}

TEST(Node, set_and_get_rotation)
{
	Node node;

	constexpr auto test{45.0F};
	node.set_rotation(test);
	EXPECT_EQ(node.get_rotation(), test);
}
