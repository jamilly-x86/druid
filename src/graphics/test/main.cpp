#include <druid/core/Engine.h>
#include <gtest/gtest.h>

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);

    // Need to ensure an engine object exists for testing purposes
    druid::core::Engine engine{};

    return RUN_ALL_TESTS();
}
