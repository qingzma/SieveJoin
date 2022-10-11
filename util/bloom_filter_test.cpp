#include <gtest/gtest.h>

namespace qjoin {
TEST(bloom_filter, test_bloom_filter) { EXPECT_STRNE("hello", "world"); }
}  // namespace qjoin