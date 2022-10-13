// NAME            | FIELD | TYPE
// O_ORDERKEY      |   0   | Identifier (int)
// O_CUSTKEY       |   1   | Identifier
// O_ORDERSTATUS   |   2   | char
// O_TOTALPRICE    |   3   | real
// O_ORDERDATE     |   4   | date (seconds since Epoch)
// O_ORDERPRIORITY |   5   | char[15]
// O_CLERK         |   6   | char[15]
// O_SHIPPRIORITY  |   7   | int
// O_COMMENT       |   8   | string[79]
#ifndef QJOIN_TABLE_TPCH_TTABLE_ORDERS_H_
#define QJOIN_TABLE_TPCH_TTABLE_ORDERS_H_
#include "qjoin/table.h"
namespace qjoin {
class TpchTableOrders : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  struct priority_struct {
    char str[16];
  };
  struct clerk_struct {
    char str[16];
  };

  std::vector<db_key_t_> order_key_;
  std::vector<db_key_t_> cust_key_;
  std::vector<char> order_status_;
  std::vector<float> total_price_;
  std::vector<db_time_t_> order_date_;
  std::vector<priority_struct> order_priority_;
  std::vector<clerk_struct> clerk_;
  std::vector<int> ship_priority_;

  std::shared_ptr<key_index_> order_index_;
  std::shared_ptr<key_index_> cust_index_;
  std::shared_ptr<int_index_> order_date_index;

 private:
  void buildOrdersIndex();
  void buildCustIndex();
  void buildOrderdateIndex();

 public:
  static const int O_ORDERKEY = 0;
  static const int O_CUSTKEY = 1;
  static const int O_ORDERSTATUS = 2;
  static const int O_TOTALPRICE = 3;
  static const int O_ORDERDATE = 4;
  static const int O_ORDERPRIORITY = 5;
  static const int O_CLERK = 6;
  static const int O_SHIPPRIORITY = 7;
  static const int O_COMMENT = 8;

  TpchTableOrders(std::string filename, int64_t row_count);
  virtual ~TpchTableOrders(){};
  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }

  DATABASE_DATA_TYPES GetColumnType(int col) const override;

  int64_t Int64At(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  std::shared_ptr<int_index_> IntIndex(int col);
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_TTABLE_ORDERS_H_