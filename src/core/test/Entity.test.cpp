#include <gtest/gtest.h>

//#ifdef _MSC_VER
//#pragma warning(push)
//#pragma warning(disable:4127) // flecs uses constexpr tests that trigger C4127
//#endif

import druid.core.engine;
import druid.core.entity;

//#ifdef _MSC_VER
//#pragma warning(pop)
//#endif

TEST(Entity, create)
{
	druid::core::Engine engine;

	auto entity = engine.create_entity();
	EXPECT_TRUE(entity.valid());
}

TEST(Entity, component)
{
	struct Transform
	{
		int x{};
		int y{};
	};

	druid::core::Engine engine;

	auto entity = engine.create_entity();
	EXPECT_FALSE(entity.has_components<Transform>());
	const auto& transform = entity.add_component<Transform>();
	EXPECT_EQ(transform.x, 0);
	EXPECT_EQ(transform.y, 0);
	EXPECT_TRUE(entity.has_components<Transform>());
	entity.remove_component<Transform>();
	EXPECT_FALSE(entity.has_components<Transform>());
}
