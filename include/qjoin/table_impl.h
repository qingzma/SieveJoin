#ifndef QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_
#define QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "column_bloom_filter.h"
#include "data_types.h"
#include "options.h"

namespace qjoin {
class TableImpl {
 protected:
  std::string table_name_;
  int64_t row_count_;
  int col_cnt_;
  bool has_col2_ = false;
  Options* options_;

 public:
  std::shared_ptr<std::vector<db_key_t_>> col0_;
  std::shared_ptr<std::vector<db_key_t_>> col1_;
  std::shared_ptr<std::vector<std::string>> col2_;

  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col0_index_;
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col1_index_;
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col0_bf_index_;
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col1_bf_index_;
  std::shared_ptr<std::vector<db_key_t_>> col0_bf_index_vec_;
  std::shared_ptr<std::vector<db_key_t_>> col1_bf_index_vec_;
  std::shared_ptr<std::multimap<std::string, int64_t>> col2_index_;

  std::shared_ptr<ColumnBloomFilter> col0_bf_;
  std::shared_ptr<ColumnBloomFilter> col1_bf_;
  std::shared_ptr<ColumnBloomFilter> col2_bf_;

 private:
  // TableImpl();

 public:
  TableImpl(Options& options, std::string filename, char delim, int col0,
            int col1);
  TableImpl(Options& options, std::string filename, char delim, int col0,
            int col1, int col3, DATABASE_DATA_TYPES col3_date_type);

  virtual ~TableImpl();

  db_key_t_ KeyValAt(int64_t row, int col) const;
  std::string StrAt(int64_t row) const;

  int64_t Size() const { return row_count_; }

  void BuildIndex();

  std::vector<db_key_t_>::iterator KeyIterator(int col) const;
  void BuildQPlusVecIndex();

 private:
  void buildKeyIndex();
  void buildCharsIndex();

 public:
  void BuildKeyBloomFilter();
  void BuildCharsBloomFilter();
};
}  // namespace qjoin
#endif  // QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_