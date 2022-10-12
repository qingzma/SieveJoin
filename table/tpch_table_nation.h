// NAME         | FIELD | TYPE
// N_NATION_KEY |   0   | Identifier (int)
// N_NAME       |   1   | Char[25]
// N_REGION_KEY |   2   | Identifier (int)
// N_COMMENT    |   3   | String[152]

#ifndef QJOIN_TABLE_TPCH_TABLE_NATION_H
#define QJOIN_TABLE_TPCH_TABLE_NATION_H

#include <string>

#include "qjoin/table.h"

namespace qjoin {
class TpchTableNation : public Table {
 public:
  static const int N_NATION_KEY = 0;
  static const int N_NATION_NAME = 1;
  static const int N_REGION_KEY = 2;
  static const int N_COMMENT = 3;
  TpchTableNation(std::string filename, size_t row_count);
  virtual ~TpchTableNation();

  int ColumnCount() const override;
  int64_t RowCount() const override;

  DATABASE_DATA_TYPES GetColumnType(int col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  const std::vector<db_key_t_>::iterator KeyIterator(int col);

 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  struct chars {
    char str[26];
  };

  std::vector<db_key_t_> nation_key_;
  std::vector<db_key_t_> region_key_;
  std::vector<chars> nation_name_;

  std::shared_ptr<key_index_> nation_index_;
  std::shared_ptr<key_index_> region_index_;

  void build_nation_index();
  void build_region_index();
};
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_TABLE_NATION_H