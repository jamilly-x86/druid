#include <gtest/gtest.h>
#include <glm/glm.hpp>

import druid.graphics.Node;

using druid::graphics::Node;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST(Node, default_constructor)
{
	auto node = std::make_unique<Node>();

	EXPECT_EQ(node->get_position(), Node::DefaultPosition);
	EXPECT_EQ(node->get_scale(), Node::DefaultScale);
	EXPECT_EQ(node->get_rotation(), Node::DefaultRotation);
	EXPECT_TRUE(std::empty(node->children()));
}

TEST(Node, create_node)
{
	auto root = std::make_unique<Node>();
	auto& node1 = root->create_node();
	auto& node2 = root->create_node();

	EXPECT_EQ(node1.parent(), root.get());
	EXPECT_EQ(node2.parent(), root.get());
	EXPECT_EQ(std::size(root->children()), 2U);
}

TEST(Node, remove_node)
{
	auto root = std::make_unique<Node>();
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

TEST(Node, set_and_get_scale)
{
	auto node = std::make_unique<Node>();
	const auto test = glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::min());
	node->set_scale(test);

	// NOLINTBEGIN
	EXPECT_EQ(node->get_scale().x, test.x);
	EXPECT_EQ(node->get_scale().y, test.y);
	// NOLINTEND
}

TEST(Node, set_and_get_rotation)
{
	auto node = std::make_unique<Node>();

	constexpr auto test{45.0F};
	node->set_rotation(test);
	EXPECT_EQ(node->get_rotation(), test);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
