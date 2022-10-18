#ifndef QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_
#define QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "data_types.h"

namespace qjoin {
class TableImpl {
 protected:
  std::string table_name_;
  int64_t row_count_;
  int col_cnt_;
  bool has_col2_ = false;

 public:
  std::shared_ptr<std::vector<db_key_t_>> col0_;
  std::shared_ptr<std::vector<db_key_t_>> col1_;
  std::shared_ptr<std::vector<std::string>> col2_;

  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col0_index_;
  std::shared_ptr<std::multimap<db_key_t_, int64_t>> col1_index_;
  std::shared_ptr<std::multimap<std::string, int64_t>> col2_index_;

 private:
  // TableImpl();

 public:
  TableImpl(std::string filename, char delim, int col0, int col1);
  TableImpl(std::string filename, char delim, int col0, int col1, int col3,
            DATABASE_DATA_TYPES col3_date_type);

  virtual ~TableImpl();

  db_key_t_ KeyValAt(int64_t row, int col) const;
  std::string StrAt(int64_t row) const;

  void BuildIndex();

 private:
  void buildKeyIndex();
  void buildCharsIndex();

  void buildKeyBloomFilter();
  void buildCharsBloomFilter();
};
}  // namespace qjoin
#endif  // QJOIN_INCLUDE_QJOIN_TABLE_IMPL_H_