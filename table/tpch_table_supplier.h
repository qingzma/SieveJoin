// NAME        | FIELD | TYPE
// S_SUPPKEY   |   0   | Identifier (int)
// S_NAME      |   1   | Char[25]
// S_ADDRESS   |   2   | String[40]
// S_NATIONKEY |   3   | Identifier (int)
// S_PHONE     |   4   | Char[15]
// S_ACCTBAL   |   5   | decimal (float?)
// S_COMMENT   |   6   | String[101]

#ifndef QJOIN_TABLE_TPCH_TABLE_SUPPLIER_H_
#define QJOIN_TABLE_TPCH_TABLE_SUPPLIER_H_

#include "qjoin/table.h"
namespace qjoin {
class TpchTableSupplier : public Table {
 private:
  std::string table_name_;
  int64_t row_count_;
  int col_count_;

  struct name_struct {
    char str[26];
  };

  struct phone_struct {
    char str[16];
  };

  std::vector<db_key_t_> supp_key_;
  std::vector<name_struct> name_;
  std::vector<db_key_t_> nation_key_;
  std::vector<phone_struct> phone_;
  std::vector<float> acctbal_;

  std::shared_ptr<key_index_> supp_index_;
  std::shared_ptr<key_index_> nation_index_;
  std::shared_ptr<float_index_> acctbal_index_;

  void buildSuppIndex();
  void buildNationIndex();
  void buildAcctbalIndex();

 public:
  static const int S_SUPPKEY = 0;
  static const int S_NAME = 1;
  static const int S_ADDRESS = 2;
  static const int S_NATIONKEY = 3;
  static const int S_PHONE = 4;
  static const int S_ACCTBAL = 5;
  static const int S_COMMENT = 6;

 public:
  TpchTableSupplier(std::string filename, size_t row_count);
  virtual ~TpchTableSupplier(){};
  int ColumnCount() const override { return col_count_; }
  int64_t RowCount() const override { return row_count_; }

  DATABASE_DATA_TYPES GetColumnType(int col) const override;
  int64_t Int64At(const int64_t& row, const int& col) const override;
  float FloatAt(const int64_t& row, const int& col) const override;
  const char* CharsAt(const int64_t& row, const int& col) const override;

  std::shared_ptr<key_index_> KeyIndex(int col) override;
  std::shared_ptr<float_index_> FloatIndex(int col);
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin

#endif  // QJOIN_TABLE_TPCH_TABLE_SUPPLIER_H_