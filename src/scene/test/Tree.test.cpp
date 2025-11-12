#include <druid/scene/Tree.h>
#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

TEST(Tree, CreateNode)
{
	druid::scene::Tree tree;
	auto node = tree.create_node();
	EXPECT_NE(node, nullptr);
}

TEST(Tree, SetTransform)
{
	druid::scene::Tree tree;
	auto node = tree.create_node();
	druid::scene::Transform transform;
	transform.local = glm::translate(glm::mat4{1.0F}, glm::vec3(1.0F, 2.0F, 3.0F));
	node->set_transform(transform);
	const auto& retrieved_transform = node->get_transform();
	EXPECT_EQ(retrieved_transform.local, transform.local);
}
