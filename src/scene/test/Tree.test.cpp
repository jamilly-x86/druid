#include <druid/scene/Tree.h>
#include <gtest/gtest.h>

TEST(Tree, CreateNode)
{
	 druid::scene::Tree tree;
	 auto node = tree.create_node();
	 EXPECT_NE(node, nullptr);
}

TEST(Tree, MakeDirty)
{
	 druid::scene::Tree tree;
	 auto node = tree.create_node();
	 node->make_dirty(druid::scene::Node::Dirty::Transform);
	 // Since dirty_ is private, we cannot access it directly.
	 // This test ensures that the method can be called without errors.
	 SUCCEED();
}
