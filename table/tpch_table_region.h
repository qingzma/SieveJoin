// NAME        | FIELD | TYPE
// R_REGIONKEY |   0   | Identifier (int)
// R_NAME      |   1   | Char[25]
// R_COMMENT   |   2   | String

#ifndef QJOIN_TABLE_TPCH_TABLE_REGION_H_
#define QJOIN_TABLE_TPCH_TABLE_REGION_H_

#include "qjoin/table.h"
namespace qjoin {
class TpchTableRegion : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  struct name_struct {
    char str[26];
  };

  std::vector<db_key_t_> region_key_;
  std::vector<name_struct> name_;

  std::shared_ptr<key_index_> region_index_;

  void buildRegionIndex();

 public:
  static const int R_REGIONKEY = 0;
  static const int R_NAME = 1;
  static const int R_COMMENT = 2;

 public:
  TpchTableRegion(std::string filename, size_t row_count);
  ~TpchTableRegion() {}
  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }

  DATABASE_DATA_TYPES GetColumnType(int col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_TABLE_REGION_H_