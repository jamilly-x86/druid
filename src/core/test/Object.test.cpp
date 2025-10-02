#include <gtest/gtest.h>

import druid.core.object;

using druid::core::Object;

TEST(Object, name)
{
	Object object;
	object.setName("test");
	EXPECT_EQ(object.getName(), "test");
}

TEST(Object, addChild)
{
	Object object;
	EXPECT_NO_THROW(object.addChild(std::make_unique<Object>()));
}

TEST(Object, createChild)
{
	Object object;

	auto& child = object.createChild("test");
	EXPECT_EQ(child.getName(), "test");
}

TEST(Object, remove)
{
	Object object;
	EXPECT_EQ(object.remove(), nullptr);

	(void)object.createChild("one");
	auto& two = object.createChild("two");
	(void)object.createChild("three");

	EXPECT_EQ(two.getName(), "two");

	EXPECT_EQ(object.children().size(), 3U);

	auto child = two.remove();
	EXPECT_EQ(child->getName(), "two");

	EXPECT_EQ(object.children().size(), 2U);
}

TEST(Object, findChild)
{
	Object object;
	auto& one = object.createChild("one");
	auto& two = object.createChild("two");
	auto& three = object.createChild("three");

	auto* child = object.findChild("one");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &one);

	child = object.findChild("two");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &two);

	child = object.findChild("three");
	ASSERT_NE(child, nullptr);
	EXPECT_EQ(child, &three);
}