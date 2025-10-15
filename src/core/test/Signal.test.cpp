#include <core/Signal.h>
#include <gtest/gtest.h>

using druid::core::Signal;

TEST(Signal, connect)
{
	Signal<void(int)> signal;

	signal.connect([](int) {});
}