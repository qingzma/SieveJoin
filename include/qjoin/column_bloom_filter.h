#ifndef QJOIN_INCLUDE_QJOIN_COLUMN_BLOOM_FILTER_H_
#define QJOIN_INCLUDE_QJOIN_COLUMN_BLOOM_FILTER_H_

#include <map>
#include <memory>

#include "util/bloom_filter.h"

#include "qjoin/data_types.h"
#include "qjoin/options.h"

namespace qjoin {
class ColumnBloomFilter {
 private:
  int level_;
  bloom_parameters parameters_;

 public:
  bloom_filter bf_;

 public:
  ColumnBloomFilter(Options& options, int64_t col_size);

  void Fit(std::shared_ptr<std::vector<db_key_t_>>& column);

  // merge the bf on column b_1 in table 1  with equal condition on column b_2
  // from another table b2.
  void UpdateBfFromOutsideColumn(
      const std::shared_ptr<std::vector<db_key_t_>>& column_inside,
      const ColumnBloomFilter& cbf);

  void UpdateBfFromOutsideColumns(
      const std::shared_ptr<std::vector<db_key_t_>>& column_inside,
      const ColumnBloomFilter& bf1, const ColumnBloomFilter& bf2);

  void UpdateBfFromInsideColumn(
      const std::shared_ptr<std::vector<db_key_t_>>& dest_col,
      const std::shared_ptr<std::vector<db_key_t_>>& source_col,
      const ColumnBloomFilter& bf_source);

  /*this method is used to support cyclic joins.
   it is a combination of the above two methods.
  */
  void UpdateBfFromInsideColumnOutsideColumn(
      const std::shared_ptr<std::vector<db_key_t_>>& dest_col,
      const std::shared_ptr<std::vector<db_key_t_>>& source_col,
      const ColumnBloomFilter& bf_source, const ColumnBloomFilter& bf_outiside);

  std::shared_ptr<std::multimap<db_key_t_, int64_t>> CreateBfIndex(
      const std::shared_ptr<std::vector<db_key_t_>>& col);

  std::shared_ptr<std::multimap<db_key_t_, int64_t>>
  CreateBfIndexWithMultipleColumns(
      const std::shared_ptr<std::vector<db_key_t_>>& col, bloom_filter bf1,
      const std::shared_ptr<std::vector<db_key_t_>>& col1);

  std::shared_ptr<std::vector<db_key_t_>> CreateBfIndexVec(
      const std::shared_ptr<std::vector<db_key_t_>>& col);
};
}  // namespace qjoin

#endif  // QJOIN_INCLUDE_QJOIN_COLUMN_BLOOM_FILTER_H_