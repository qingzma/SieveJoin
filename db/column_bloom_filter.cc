#include "qjoin/column_bloom_filter.h"

#include <cassert>
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
  bf_ = bloom_filter(parameters_);

  level_ = 0;
}

void ColumnBloomFilter::Fit(std::shared_ptr<std::vector<db_key_t_>>& column) {
  for (auto val : *column) bf_.insert(val);
}

void ColumnBloomFilter::UpdateBfFromOutsideColumn(
    const std::shared_ptr<std::vector<db_key_t_>>& column_inside,
    const ColumnBloomFilter& cbf_outside) {
  // reset Bloom Filter
  bf_.clear();

  for (auto val : *column_inside) {
    if (cbf_outside.bf_.contains(val)) bf_.insert(val);
  }
}

void ColumnBloomFilter::UpdateBfFromOutsideColumns(
    const std::shared_ptr<std::vector<db_key_t_>>& column_inside,
    const ColumnBloomFilter& bf1, const ColumnBloomFilter& bf2) {
  // reset Bloom Filter
  bf_.clear();

  for (auto val : *column_inside) {
    if (bf1.bf_.contains(val) && bf2.bf_.contains(val)) bf_.insert(val);
  }
}

void ColumnBloomFilter::UpdateBfFromInsideColumn(
    const std::shared_ptr<std::vector<db_key_t_>>& dest_col,
    const std::shared_ptr<std::vector<db_key_t_>>& source_col,
    const ColumnBloomFilter& bf_source) {
  // reset Bloom Filter
  bf_.clear();

  for (int64_t idx = 0; idx < dest_col->size(); idx++) {
    if (bf_source.bf_.contains(source_col->at(idx))) {
      bf_.insert(dest_col->at(idx));
    }
  }
}

void ColumnBloomFilter::UpdateBfFromInsideColumnOutsideColumn(
    const std::shared_ptr<std::vector<db_key_t_>>& dest_col,
    const std::shared_ptr<std::vector<db_key_t_>>& source_col,
    const ColumnBloomFilter& bf_source, const ColumnBloomFilter& bf_outiside) {
  assert(bf_outiside.bf_.element_count() == bf_source.bf_.element_count());
  // reset Bloom Filter
  bf_.clear();

  for (int64_t idx = 0; idx < dest_col->size(); idx++) {
    if (bf_source.bf_.contains(source_col->at(idx)) &&
        bf_outiside.bf_.contains(dest_col->at(idx))) {
      bf_.insert(dest_col->at(idx));
    }
  }
}

std::shared_ptr<std::multimap<db_key_t_, int64_t>>
ColumnBloomFilter::CreateBfIndex(
    const std::shared_ptr<std::vector<db_key_t_>>& col) {
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> idx =
      std::make_shared<std::multimap<db_key_t_, int64_t>>();

  for (int64_t i = 0; i < col->size(); i++) {
    if (bf_.contains(col->at(i))) idx->emplace(col->at(i), i);
  }

  return idx;
}

std::shared_ptr<std::multimap<db_key_t_, int64_t>>
ColumnBloomFilter::CreateBfIndexWithMultipleColumns(
    const std::shared_ptr<std::vector<db_key_t_>>& col, bloom_filter bf1,
    const std::shared_ptr<std::vector<db_key_t_>>& col1) {
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> idx =
      std::make_shared<std::multimap<db_key_t_, int64_t>>();
  std::cout << "was " << col->size();
  for (int64_t i = 0; i < col->size(); i++) {
    if (bf_.contains(col->at(i)) && bf1.contains(col1->at(i)))
      idx->emplace(col->at(i), i);
  }
  std::cout << ", changed to " << idx->size() << std::endl;

  return idx;
}

std::shared_ptr<std::vector<db_key_t_>> ColumnBloomFilter::CreateBfIndexVec(
    const std::shared_ptr<std::vector<db_key_t_>>& col) {
  std::shared_ptr<std::vector<db_key_t_>> idx =
      std::make_shared<std::vector<db_key_t_>>();

  for (int64_t i = 0; i < col->size(); i++) {
    if (bf_.contains(col->at(i))) idx->emplace_back(col->at(i));
  }

  return idx;
}

}  // namespace qjoin
