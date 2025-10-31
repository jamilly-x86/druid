#include <gtest/gtest.h>

#include <druid/entity/World.h>

TEST(Word, create_entity)
{
	druid::entity::World world;
	const auto entity1 = world.create_entity();
	const auto entity2 = world.create_entity();
	EXPECT_NE(entity1, entity2);
}
