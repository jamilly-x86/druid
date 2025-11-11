#include <druid/scene/Tree.h>
#include <gtest/gtest.h>

TEST(Tree, CreateNode)
{
	 druid::scene::Tree tree;
	 auto node = tree.create_node();
	 EXPECT_NE(node, nullptr);
}
