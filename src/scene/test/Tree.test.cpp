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

	constexpr auto position = glm::vec3{1.0F, 2.0F, 3.0F};
	transform.local = glm::translate(glm::mat4{1.0F}, position);
	node->set_transform(transform);
	tree.update_transforms();
	const auto& retrieved_transform = node->get_transform();
	EXPECT_EQ(retrieved_transform.local, transform.local);
	EXPECT_EQ(retrieved_transform.global, transform.local);
}

TEST(Tree, NestedTransform)
{
	druid::scene::Tree tree;
	auto parent_node = tree.create_node();
	auto child_node = tree.create_node();

	constexpr auto pos1 = glm::vec3(2.0F, 0.0F, 0.0F);
	constexpr auto pos2 = glm::vec3(0.0F, 2.0F, 0.0F);

	parent_node->set_transform({.local = glm::translate(glm::mat4{1.0F}, pos1)});
	child_node->set_transform({.local = glm::translate(glm::mat4{1.0F}, pos2)});
	parent_node->add_child(*child_node);
	tree.update_transforms();
	const auto& parent_transform = parent_node->get_transform();
	const auto& child_transform = child_node->get_transform();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(parent_transform.global, scale, rotation, translation, skew, perspective);
	EXPECT_EQ(translation, pos1);
	glm::decompose(child_transform.global, scale, rotation, translation, skew, perspective);
	EXPECT_EQ(translation, pos1 + pos2);
}

TEST(Tree, RemoveChild)
{
	druid::scene::Tree tree;
	auto parent_node = tree.create_node();
	auto child_node = tree.create_node();
	constexpr auto pos1 = glm::vec3(2.0F, 0.0F, 0.0F);
	constexpr auto pos2 = glm::vec3(0.0F, 2.0F, 0.0F);
	parent_node->set_transform({.local = glm::translate(glm::mat4{1.0F}, pos1)});
	child_node->set_transform({.local = glm::translate(glm::mat4{1.0F}, pos2)});
	parent_node->add_child(*child_node);
	tree.update_transforms();
	parent_node->remove_child(*child_node);
	tree.update_transforms();
	const auto& child_transform = child_node->get_transform();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(child_transform.global, scale, rotation, translation, skew, perspective);
	EXPECT_EQ(translation, pos2);
}