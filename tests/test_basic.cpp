#include <gtest/gtest.h>

// A trivial test to ensure your build/test pipeline works
TEST(SanityCheck, TrueIsTrue) {
    ASSERT_TRUE(true);
}

// Main function required for GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 