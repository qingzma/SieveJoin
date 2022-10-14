#include "qjoin/table_generic.h"

namespace qjoin {
void TableGenericBase::BuildIndex(bool for_all) {
  // build index 1
  if (!for_all) {
    index_element_ tmp;
    tmp.idx = 0;
    for (auto& i : col1_) {
      tmp.val = i;
      col1_index_.push_back(tmp);
      ++tmp.idx;
    }
    // sort index by key
    std::sort(col1_index_.begin(), col1_index_.end(),
              [](const index_element_& a, const index_element_& b) {
                return a.val < b.val;
              });

    // build index 2
    if (!for_all && &col1_ != &col2_) {
      index_element_ tmp;
      tmp.idx = 0;
      for (auto& i : col2_) {
        tmp.val = i;
        col2_index_.push_back(tmp);
        ++tmp.idx;
      }
      // sort index by key
      std::sort(col2_index_.begin(), col2_index_.end(),
                [](const index_element_& a, const index_element_& b) {
                  return a.val < b.val;
                });
    }
  } else {
    throw std::runtime_error("build indexes for all is not supported.");
  }
}

int TableGenericBase::Size() { return std::max(col1_.size(), col2_.size()); }

int TableGenericBase::CountCardinality(int val, int col) {
  switch (col) {
    case 1: {
      auto start_index = std::lower_bound(
          col1_index_.begin(), col1_index_.end(), val,
          [](const index_element_& a, int b) { return a.val < b; });
      auto end_index = std::upper_bound(
          start_index, col1_index_.end(), val,
          [](int a, const index_element_& b) { return a < b.val; });
      return (int)std::distance(start_index, end_index);
      break;
    }
    case 2: {
      auto start_idx = std::lower_bound(
          col2_index_.begin(), col2_index_.end(), val,
          [](const index_element_& a, int b) { return a.val < b; });
      auto end_idx = std::upper_bound(
          start_idx, col2_index_.end(), val,
          [](int a, const index_element_& b) { return a < b.val; });
      return (int)std::distance(start_idx, end_idx);
      break;
    }
    default:
      throw std::runtime_error("column does not exist.");
  }
}

db_key_t_ TableGenericBase::Col1Val(int index) { return col1_[index]; }

db_key_t_ TableGenericBase::Col2Val(int index) { return col2_[index]; }
}  // namespace qjoin