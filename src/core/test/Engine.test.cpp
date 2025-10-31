#include <gtest/gtest.h>

#include <druid/core/Engine.h>

using druid::core::Engine;

TEST(Engine, default_constructor)
{
	const Engine engine;
	EXPECT_EQ(engine.get_interval_fixed(), Engine::DefaultIntervalFixed);
}

TEST(Engine, set_interval_fixed)
{
	Engine engine;
	engine.set_interval_fixed(std::numeric_limits<std::chrono::milliseconds>::max());
	EXPECT_EQ(engine.get_interval_fixed(), std::numeric_limits<std::chrono::milliseconds>::max());
}
