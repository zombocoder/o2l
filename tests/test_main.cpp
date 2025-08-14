#include <gtest/gtest.h>

// Main function for Google Test
// This is automatically provided by gtest_main, but we include it for completeness
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}