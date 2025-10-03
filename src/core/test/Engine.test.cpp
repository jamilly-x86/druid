#include <gtest/gtest.h>

import druid.core.engine;

using druid::core::Engine;

TEST(Engine, default_constructor)
{
	const druid::core::Engine engine;
	EXPECT_EQ(engine.get_interval_fixed(), druid::core::Engine::DefaultIntervalFixed);
}

TEST(Engine, set_interval_fixed)
{
	druid::core::Engine engine;
	engine.set_interval_fixed(std::chrono::milliseconds{20});
	EXPECT_EQ(engine.get_interval_fixed(), std::chrono::milliseconds{20});
}

TEST(Engine, update_count)
{
	druid::core::Engine engine;
	EXPECT_EQ(engine.update_fixed_count(), 0U);

	// Use 99 instead of 100 as we want the 100th update to trigger the exit.
	constexpr auto exit_condition{99U};

	std::uint64_t count{};
	engine.on_update_fixed(
		[&engine, &count](auto)
		{
			if (count == exit_condition)
			{
				engine.quit();
			}

			count++;
		});

	const auto start = std::chrono::steady_clock::now();
	engine.run();
	const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
	EXPECT_NEAR(elapsed.count(), std::chrono::duration<double>{1.0}.count(), 1e-6);
	EXPECT_EQ(engine.update_fixed_count(), count);
}