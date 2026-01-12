#include <gtest/gtest.h>
#include <glm/glm.hpp>

import druid.core.Engine;
import druid.graphics.Node;

using druid::core::Engine;
using druid::graphics::Node;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST(Node, default_constructor)
{
	Engine engine;
	auto node = std::make_unique<Node>();

	EXPECT_EQ(node->get_position(), Node::DefaultPosition);
	EXPECT_EQ(node->get_scale(), Node::DefaultScale);
	EXPECT_EQ(node->get_rotation(), Node::DefaultRotation);
	EXPECT_TRUE(std::empty(node->children()));
}

TEST(Node, create_node)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	auto& node1 = root->create_node();
	auto& node2 = root->create_node();

	EXPECT_EQ(node1.parent(), root.get());
	EXPECT_EQ(node2.parent(), root.get());
	EXPECT_EQ(std::size(root->children()), 2U);
}

TEST(Node, remove_node)
{
	Engine engine;
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

TEST(Node, get_position_global)
{
	Engine engine;
	auto root = std::make_unique<Node>();
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
	Engine engine;
	auto node = std::make_unique<Node>();

	constexpr auto test{45.0F};
	node->set_rotation(test);
	EXPECT_EQ(node->get_rotation(), test);
}

TEST(Node, transform_cache_invalidation_on_parent_change)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	Node& child = root->create_node();
	Node& grandchild = child.create_node();

	// Set initial positions
	root->set_position({10.0F, 10.0F});
	child.set_position({5.0F, 5.0F});
	grandchild.set_position({2.0F, 2.0F});

	// Get initial global position - this caches the global transform
	auto initial_pos = grandchild.get_position_global();
	EXPECT_FLOAT_EQ(initial_pos.x, 17.0F);
	EXPECT_FLOAT_EQ(initial_pos.y, 17.0F);

	// Change parent position - should invalidate child caches
	root->set_position({20.0F, 20.0F});

	// Global position should reflect the change
	auto updated_pos = grandchild.get_position_global();
	EXPECT_FLOAT_EQ(updated_pos.x, 27.0F);
	EXPECT_FLOAT_EQ(updated_pos.y, 27.0F);

	// Change middle child position
	child.set_position({10.0F, 10.0F});

	auto final_pos = grandchild.get_position_global();
	EXPECT_FLOAT_EQ(final_pos.x, 32.0F);
	EXPECT_FLOAT_EQ(final_pos.y, 32.0F);
}

TEST(Node, transform_consistency_after_removing_node)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	Node& child1 = root->create_node();
	Node& child2 = root->create_node();
	Node& grandchild = child1.create_node();

	root->set_position({10.0F, 10.0F});
	child1.set_position({5.0F, 5.0F});
	child2.set_position({3.0F, 3.0F});
	grandchild.set_position({2.0F, 2.0F});

	// Grandchild's global position with parent chain
	auto pos_with_parent = grandchild.get_position_global();
	EXPECT_FLOAT_EQ(pos_with_parent.x, 17.0F);
	EXPECT_FLOAT_EQ(pos_with_parent.y, 17.0F);

	// Remove child1 from hierarchy
	auto removed = child1.remove();
	ASSERT_NE(removed, nullptr);

	// Grandchild should now have global position equal to its local + child1's local
	// (no longer includes root)
	auto pos_after_removal = grandchild.get_position_global();
	EXPECT_FLOAT_EQ(pos_after_removal.x, 7.0F);
	EXPECT_FLOAT_EQ(pos_after_removal.y, 7.0F);

	// child2 should still have correct global position
	auto child2_pos = child2.get_position_global();
	EXPECT_FLOAT_EQ(child2_pos.x, 13.0F);
	EXPECT_FLOAT_EQ(child2_pos.y, 13.0F);
}

TEST(Node, transform_consistency_after_adding_node)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	auto orphan_parent = std::make_unique<Node>();
	Node& orphan_child = orphan_parent->create_node();
	Node* orphan_parent_ptr = orphan_parent.get();

	root->set_position({10.0F, 10.0F});
	orphan_parent->set_position({5.0F, 5.0F});
	orphan_child.set_position({2.0F, 2.0F});

	// Initial orphan global position (only includes orphan_parent, no root)
	auto initial_pos = orphan_child.get_position_global();
	EXPECT_FLOAT_EQ(initial_pos.x, 7.0F);
	EXPECT_FLOAT_EQ(initial_pos.y, 7.0F);

	// Move orphan_parent into root hierarchy
	root->add_child(std::move(orphan_parent));

	// Now orphan_child's global position should include root
	auto new_pos = orphan_child.get_position_global();
	EXPECT_FLOAT_EQ(new_pos.x, 17.0F);
	EXPECT_FLOAT_EQ(new_pos.y, 17.0F);

	// Verify parent relationship is correct
	EXPECT_EQ(orphan_parent_ptr->parent(), root.get());
	EXPECT_EQ(orphan_child.parent(), orphan_parent_ptr);
}

TEST(Node, multiple_property_changes_cache_correctly)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	Node& child = root->create_node();

	root->set_position({10.0F, 10.0F});
	child.set_position({5.0F, 5.0F});

	// Get initial global position
	auto pos1 = child.get_position_global();

	// Change multiple properties on root
	root->set_position({20.0F, 20.0F});
	root->set_scale({2.0F, 2.0F});
	root->set_rotation(90.0F);

	// Child's global position should reflect all changes
	auto pos2 = child.get_position_global();

	// Position should be different
	EXPECT_NE(pos1.x, pos2.x);
	EXPECT_NE(pos1.y, pos2.y);

	// Verify transform is actually updated
	auto transform = child.transform_global();
	EXPECT_NE(transform, glm::mat4(1.0F));
}

TEST(Node, deep_hierarchy_transform_propagation)
{
	Engine engine;
	auto root = std::make_unique<Node>();

	// Create a deep hierarchy
	Node* current = root.get();
	constexpr int depth = 10;

	for (int i = 0; i < depth; ++i)
	{
		current = &current->create_node();
		current->set_position({1.0F, 1.0F});
	}

	// Bottom node should have global position of (10, 10)
	auto bottom_pos = current->get_position_global();
	EXPECT_FLOAT_EQ(bottom_pos.x, static_cast<float>(depth));
	EXPECT_FLOAT_EQ(bottom_pos.y, static_cast<float>(depth));

	// Change root position - should propagate all the way down
	root->set_position({5.0F, 5.0F});

	auto updated_pos = current->get_position_global();
	EXPECT_FLOAT_EQ(updated_pos.x, static_cast<float>(depth + 5));
	EXPECT_FLOAT_EQ(updated_pos.y, static_cast<float>(depth + 5));
}

TEST(Node, scale_affects_child_global_position)
{
	Engine engine;
	auto root = std::make_unique<Node>();
	Node& child = root->create_node();

	root->set_position({0.0F, 0.0F});
	root->set_scale({2.0F, 2.0F});
	child.set_position({10.0F, 10.0F});

	// With parent scale of 2, child's global position should be scaled
	auto global_pos = child.get_position_global();
	EXPECT_FLOAT_EQ(global_pos.x, 20.0F);
	EXPECT_FLOAT_EQ(global_pos.y, 20.0F);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
