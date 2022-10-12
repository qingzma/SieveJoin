// NAME             |  FIELD | TYPE
// C_CUSTKEY        |    0   | Identifier (int)
// C_NAME           |    1   | String[25]
// C_ADDRESS        |    2   | String[40]
// C_NATIONKEY      |    3   | Identifier
// C_PHONE          |    4   | CHAR[15]
// C_ACCTBAL        |    5   | real
// C_MKTSEGMENT     |    6   | CHAR[10]
// C_COMMENT        |    7   | String[117]
#ifndef _QJOIN_TPCH_TABLE_CUSTOMER_H
#define _QJOIN_TPCH_TABLE_CUSTOMER_H
#include "qjoin/data_types.h"
#include "qjoin/table.h"

namespace qjoin {
class TpchTableCustomer : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;
  struct phone_str {
    char str[16];
  };

  struct mktsegment_str {
    char str[11];
  };

  std::vector<db_key_t_> custkey_;
  std::vector<db_key_t_> nationkey_;
  std::vector<phone_str> phone_;
  std::vector<float> acctbal_;
  std::vector<mktsegment_str> mktsegment_;

  std::shared_ptr<key_index_> cust_index_;
  std::shared_ptr<key_index_> nation_index_;
  std::shared_ptr<float_index_> acctbal_index_;

  void buildCustIndex();
  void buildNationIndex();
  void buildAcctbalIndex();

 public:
  static const int C_CUSTKEY = 0;
  static const int C_NAME = 1;
  static const int C_ADDRESS = 2;
  static const int C_NATIONKEY = 3;
  static const int C_PHONE = 4;
  static const int C_ACCTBAL = 5;
  static const int C_MKTSEGMENT = 6;
  static const int C_COMMENT = 7;

  TpchTableCustomer(std::string filename, size_t row_count);
  virtual ~TpchTableCustomer();

  int ColumnCount() const override;
  int64_t RowCount() const override;
  DATABASE_DATA_TYPES GetColumnType(int col) const override;

  int64_t Int64At(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  std::shared_ptr<float_index_> FloatIndex(int col);
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin

#endif  //_QJOIN_TPCH_TABLE_CUSTOMER_H