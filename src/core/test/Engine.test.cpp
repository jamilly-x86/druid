#include <gtest/gtest.h>

import druid.core.engine;
import druid.core.entity;

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
