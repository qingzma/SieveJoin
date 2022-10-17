// NAME          | FIELD | TYPE
// P_PARTKEY     |   0   | Identifier
// P_NAME        |   1   | string[55]
// P_MFGR        |   2   | char[25]
// P_BRAND       |   3   | char[10]
// P_TYPE        |   4   | string[25]
// P_SIZE        |   5   | int
// P_CONTAINER   |   6   | char[10]
// P_RETAILPRICE |   7   | float
// P_COMMENT     |   8   | string[23]

#ifndef QJOIN_TABLE_TPCH_TABLE_PART_H_
#define QJOIN_TABLE_TPCH_TABLE_PART_H_

#include "qjoin/table.h"
#include "tpch_schema_columns.h"
namespace qjoin {

class TpchTablePart : public Table {
 public:
  TpchTablePart(std::string filename, size_t row_count);
  virtual ~TpchTablePart(){};

  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }

  DATABASE_DATA_TYPES GetColumnType(int col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;
  int32_t IntAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  const std::vector<db_key_t_>::iterator KeyIterator(int col);

 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;
  std::vector<db_key_t_> part_key_;
  std::vector<int> size_;
  std::vector<float> retail_price_;
  std::shared_ptr<key_index_> part_index_;

  void buildPartkeyIndex();
};
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_TABLE_PART_H_