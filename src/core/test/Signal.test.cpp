#include <gtest/gtest.h>

import druid.core.signal;

using druid::core::Signal;

TEST(Signal, connect)
{
	Signal<void(int)> signal;

	signal.connect([](int) {});
}