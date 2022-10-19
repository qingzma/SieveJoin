#include "qjoin/column_bloom_filter.h"

#include <iostream>
namespace qjoin {
ColumnBloomFilter::ColumnBloomFilter(Options& options, int64_t col_size) {
  // How many elements roughly do we expect to insert?
  parameters_.projected_element_count = col_size;

  // Maximum tolerable false positive probability? (0,1)
  parameters_.false_positive_probability = 0.001;  // 1 in 1000

  // Simple randomizer (optional)
  //   parameters_.random_seed = 0xA5A5A5A5;

  if (!parameters_) {
    std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
    return;
  }

  parameters_.compute_optimal_parameters();

  // Instantiate Bloom Filter
  bf = std::make_shared<bloom_filter>(parameters_);

  level_ = 0;
}

void ColumnBloomFilter::Fit(std::shared_ptr<std::vector<db_key_t_>>& column) {
  for (auto val : *column) bf->insert(val);
}

void ColumnBloomFilter::UpdateBfFromOutsideColumn(
    const std::shared_ptr<std::vector<db_key_t_>>& column,
    const ColumnBloomFilter& cbf) {
  // reset Bloom Filter
  bf->clear();

  for (auto val : *column) {
    if (cbf.bf->contains(val)) bf->insert(val);
  }
}

void ColumnBloomFilter::UpdateBfFromInsideColumn(
    const std::shared_ptr<std::vector<db_key_t_>>& dest_col,
    const std::shared_ptr<std::vector<db_key_t_>>& source_col,
    const ColumnBloomFilter& bf_source) {
  // reset Bloom Filter
  bf->clear();

  for (int64_t idx = 0; idx < dest->size(); idx++) {
    if (bf_source.bf->contains(source->at(idx))) {
      bf->insert(dest->at(idx));
    }
  }
}

}  // namespace qjoin
