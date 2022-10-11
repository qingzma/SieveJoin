#include "bloom_filter.h"

#include <gtest/gtest.h>

namespace qjoin {
TEST(bloom_filter, test_bloom_filter) {
  //   EXPECT_STRNE("hello", "world");
  {
    bloom_parameters parameters;

    // How many elements roughly do we expect to insert?
    parameters.projected_element_count = 1000;

    // Maximum tolerable false positive probability? (0,1)
    parameters.false_positive_probability = 0.0001;  // 1 in 10000

    // Simple randomizer (optional)
    parameters.random_seed = 0xA5A5A5A5;

    if (!parameters) {
      std::cout << "Error - Invalid set of bloom filter parameters!"
                << std::endl;
      return;
    }

    parameters.compute_optimal_parameters();

    // Instantiate Bloom Filter
    bloom_filter filter(parameters);

    std::string str_list[] = {"AbC", "iJk", "XYZ"};
    // Insert into Bloom Filter
    {
      // Insert some strings
      for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string));
           ++i) {
        filter.insert(str_list[i]);
      }

      // Insert some numbers
      for (std::size_t i = 0; i < 100; ++i) {
        filter.insert(i);
      }
    }

    // Query Bloom Filter
    {
      // Query the existence of strings
      for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string));
           ++i) {
        EXPECT_TRUE(filter.contains(str_list[i]));
      }

      // Query the existence of numbers
      for (std::size_t i = 0; i < 100; ++i) {
        EXPECT_TRUE(filter.contains(i));
      }

      // Query the existence of invalid numbers
      for (int i = -1; i > -100; --i) {
        EXPECT_FALSE(filter.contains(i));
      }
    }
  }
}
}  // namespace qjoin