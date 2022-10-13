// NAME          | FIELD | TYPE
// PS_PARTKEY    |   0   | Identifier (int)
// PS_SUPPKEY    |   1   | Identifier (int)
// PS_AVAILQTY   |   2   | int32
// PS_SUPPLYCOST |   3   | float
// PS_COMMENT    |   4   | string[199]

#ifndef _QJOIN_TABLE_TPCH_TABLE_PART_SUPP_H_
#define _QJOIN_TABLE_TPCH_TABLE_PART_SUPP_H_
#include "qjoin/table.h"

namespace qjoin {
class TpchTablePartsupp : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  std::vector<db_key_t_> part_key_;
  std::vector<db_key_t_> supp_key_;
  std::vector<int32_t> avail_qty_;
  std::vector<float> supply_cost_;

  std::shared_ptr<key_index_> part_index_;
  std::shared_ptr<key_index_> supp_index_;

 private:
  void buildPartIndex();
  void buildSuppIndex();

 public:
  static const int PS_PARTKEY = 0;
  static const int PS_SUPPKEY = 1;
  static const int PS_AVAILQTY = 2;
  static const int PS_SUPPLYCOST = 3;
  static const int PS_COMMENT = 4;

 public:
  TpchTablePartsupp(std::string filename, size_t row_count);
  ~TpchTablePartsupp() {}

  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }

  DATABASE_DATA_TYPES GetColumnType(int col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;
  int32_t IntAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};

}  // namespace qjoin

#endif  //_QJOIN_TABLE_TPCH_TABLE_PART_SUPP_H_