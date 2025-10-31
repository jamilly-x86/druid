#include <gtest/gtest.h>

#include <druid/core/Signal.h>

using druid::core::Signal;

TEST(Signal, connect)
{
	Signal<void(int)> signal;

	signal.connect([](int) {});
}