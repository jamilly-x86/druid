#include <gtest/gtest.h>

import druid.core.signal;

using druid::core::Signal;

TEST(Signal, connect)
{
	Signal<int> signal;

	signal.connect([](int) {});
}