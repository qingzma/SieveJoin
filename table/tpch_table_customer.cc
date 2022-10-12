#include "tpch_table_customer.h"

namespace qjoin {
TpchTableCustomer::TpchTableCustomer(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  custkey_.resize(row_count);
  nationkey_.resize(row_count);
  phone_.resize(row_count);
  acctbal_.resize(row_count);
  mktsegment_.resize(row_count);
  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    size_t start = 0;
    size_t next = input_buffer.find(COL_DELIMITER, 0);

    custkey_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // WE WILL NOT BE STORING THE NAME VALUE NOW
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    //  WE WILL NOT BE STORING ADDRESS VALUE NOW
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    nationkey_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(phone_[index].str, &input_buffer[start], next - start);
    phone_[index].str[next - start + 1] = 0;
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    acctbal_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(mktsegment_[index].str, &input_buffer[start], next - start);
    mktsegment_[index].str[next - start + 1] = 0;

    // WE WILL NOT BE STORING COMMENT VALUE NOW
    ++index;
  }
  input_file.close();

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

TpchTableCustomer::~TpchTableCustomer() {}

DATABASE_DATA_TYPES TpchTableCustomer::GetColumnType(int col) const {
  switch (col) {
    case C_CUSTKEY:
    case C_NATIONKEY:
      return DATABASE_DATA_TYPES::INT64;
    case C_NAME:
    case C_ADDRESS:
    case C_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    case C_PHONE:
    case C_MKTSEGMENT:
      return DATABASE_DATA_TYPES::CHAR;
    case C_ACCTBAL:
      return DATABASE_DATA_TYPES::FLOAT;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTableCustomer::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case C_CUSTKEY:
      return custkey_[row];
    case C_NATIONKEY:
      return nationkey_[row];
    default:
      throw std::runtime_error("fetching this column is not implemented.");
  }
}

const char* TpchTableCustomer::CharsAt(const int64_t& row,
                                       const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case C_PHONE:
      return phone_[row].str;
    case C_MKTSEGMENT:
      return mktsegment_[row].str;
    default:
      throw std::runtime_error("fetching this column is not implemented.");
  }
}

float TpchTableCustomer::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case C_ACCTBAL:
      return acctbal_[row];
    default:
      throw std::runtime_error("fetching this column is not implemented.");
  }
}

void TpchTableCustomer::buildCustIndex() {
  if (cust_index_ != nullptr) return;
  cust_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : custkey_) cust_index_->emplace(val, index++);
}

void TpchTableCustomer::buildNationIndex() {
  if (nation_index_ != nullptr) return;
  nation_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : nationkey_) nation_index_->emplace(val, index++);
}

void TpchTableCustomer::buildAcctbalIndex() {
  if (acctbal_index_ != nullptr) return;
  acctbal_index_.reset(new float_index_());

  int64_t index = 0;
  for (float val : acctbal_) acctbal_index_->emplace(val, index++);
}

const std::vector<db_key_t_>::iterator TpchTableCustomer::KeyIterator(int col) {
  switch (col) {
    case C_CUSTKEY:
      return custkey_.begin();
    case C_NATIONKEY:
      return nationkey_.begin();
    default:
      throw std::runtime_error("fetching this column is not implemented.");
  }
}

std::shared_ptr<key_index_> TpchTableCustomer::KeyIndex(int col) {
  switch (col) {
    case C_CUSTKEY:
      buildCustIndex();
      return cust_index_;
    case C_NATIONKEY:
      buildNationIndex();
      return nation_index_;
    default:
      throw std::runtime_error(
          "The requested column does not have this type of index");
  }
}

std::shared_ptr<float_index_> TpchTableCustomer::FloatIndex(int col) {
  switch (col) {
    case C_ACCTBAL:
      buildAcctbalIndex();
      return acctbal_index_;
    default:
      throw std::runtime_error(
          "The requested column does not have this type of index");
  }
}

int TpchTableCustomer::ColumnCount() const { return col_count_; }
int64_t TpchTableCustomer::RowCount() const { return row_count_; }

}  // namespace qjoin