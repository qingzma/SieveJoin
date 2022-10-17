// NAME             | FIELD | TYPE
// L_ORDERKEY       |   0   | Identifier
// L_PARTKEY        |   1   | Identifier
// L_SUPPKEY        |   2   | Identifier
// L_LINENUMBER     |   3   | Integer
// L_QUANTITY       |   4   | real
// L_EXTENDEDPRICE  |   5   | real
// L_DISCOUNT       |   6   | real
// L_TAX            |   7   | real
// L_RETURNFLAG     |   8   | char[1]
// L_LINESTATUS     |   9   | char[1]
// L_SHIPDATE       |  10   | date (seconds since Epoch)
// L_COMMITDATE     |  11   | date (seconds since Epoch)
// L_RECEIPTDATE    |  12   | date (seconds since Epoch)
// L_SHIPINSTRUCT   |  13   | char[25]
// L_SHIPMODE       |  14   | char[10]
// L_COMMENT        |  15   | string[44]

#ifndef QJOIN_TPCH_TABLE_LINEITEM_H_
#define QJOIN_TPCH_TABLE_LINEITEM_H_

#include "qjoin/table.h"
#include "tpch_schema_columns.h"

namespace qjoin {
class TpchTableLineitem : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  struct ship_instruct_struct {
    char str[26];
  };
  struct ship_mode_struct {
    char str[11];
  };
  std::vector<db_key_t_> order_key_;
  std::vector<db_key_t_> part_key_;
  std::vector<db_key_t_> supp_key_;
  std::vector<int> line_number_;
  std::vector<float> quantity_;
  std::vector<float> extended_price_;
  std::vector<float> discount_;
  std::vector<float> tax_;
  std::vector<char> return_flag_;
  std::vector<char> line_status_;
  std::vector<db_time_t_> ship_date_;
  std::vector<db_time_t_> commit_date_;
  std::vector<db_time_t_> receipt_date_;
  std::vector<ship_instruct_struct> ship_instruct_;
  std::vector<ship_mode_struct> ship_mode_;

  std::shared_ptr<key_index_> order_index_;
  std::shared_ptr<key_index_> part_index_;
  std::shared_ptr<key_index_> supp_index_;

  std::shared_ptr<int_index_> ship_date_index;
  std::shared_ptr<int_index_> commit_date_index;
  std::shared_ptr<int_index_> receipt_date_index;

  void buildOrderIndex();
  void buildPartIndex();
  void buildSuppIndex();

  void buildShipDateIndex();
  void buildCommitDateIndex();
  void buildReceiptDateIndex();

 public:
  TpchTableLineitem(std::string filename, int64_t row_count);
  virtual ~TpchTableLineitem();
  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }
  DATABASE_DATA_TYPES GetColumnType(int col) const override;

  int32_t IntAt(const int64_t& row, const int& col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  std::shared_ptr<int_index_> IntIndex(int col);
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin

#endif  // QJOIN_TPCH_TABLE_LINEITEM_H_