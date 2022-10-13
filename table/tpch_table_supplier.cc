#include "tpch_table_supplier.h"

#include <regex>
namespace qjoin {

TpchTableSupplier::TpchTableSupplier(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  supp_key_.resize(row_count);
  name_.resize(row_count);
  nation_key_.resize(row_count);
  phone_.resize(row_count);
  acctbal_.resize(row_count);

  // build the scanf string to use to read the file
  std::string formater = "(\\d+)\\";
  formater += COL_DELIMITER;

  formater += "([[:print:]]{0,25})\\";
  formater += COL_DELIMITER;

  formater += "([[:print:]]{0,40})\\";
  formater += COL_DELIMITER;

  formater += "(\\d+)\\";
  formater += COL_DELIMITER;

  formater += "([[:print:]]{0,15})\\";
  formater += COL_DELIMITER;

  formater += "(\\-?\\d+|\\-?\\d+.\\d+)\\";
  formater += COL_DELIMITER;

  formater += "([[:print:]]{0,101})\\";
  formater += COL_DELIMITER;

  std::smatch matcher;

  std::regex pattern(formater);

  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    if (regex_search(input_buffer, matcher, pattern)) {
      supp_key_[index] = strtoll(matcher[1].str().c_str(), nullptr, 10);
      strcpy(name_[index].str, matcher[2].str().c_str());
      // ignore index 3, which is the address
      nation_key_[index] = strtoll(matcher[4].str().c_str(), nullptr, 10);
      strcpy(phone_[index].str, matcher[5].str().c_str());
      acctbal_[index] = strtof(matcher[6].str().c_str(), nullptr);

      ++index;
      // discard the last field (no support for varchar!)
    }
  }

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

DATABASE_DATA_TYPES TpchTableSupplier::GetColumnType(int col) const {
  switch (col) {
    case S_SUPPKEY:
    case S_NATIONKEY:
      return DATABASE_DATA_TYPES::INT64;
    case S_NAME:
    case S_PHONE:
      return DATABASE_DATA_TYPES::CHAR;
    case S_ADDRESS:
    case S_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    case S_ACCTBAL:
      return DATABASE_DATA_TYPES::FLOAT;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}
int64_t TpchTableSupplier::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case S_SUPPKEY:
      return supp_key_[row];
    case S_NATIONKEY:
      return nation_key_[row];
    default:
      throw std::runtime_error("this column is not of int64 type.");
  }
}
float TpchTableSupplier::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == S_ACCTBAL);
  return acctbal_[row];
}
const char* TpchTableSupplier::CharsAt(const int64_t& row,
                                       const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case S_NAME:
      return name_[row].str;
    case S_PHONE:
      return phone_[row].str;
    default:
      throw std::runtime_error("this column is not of char type.");
  }
}

std::shared_ptr<key_index_> TpchTableSupplier::KeyIndex(int col) {
  switch (col) {
    case S_SUPPKEY:
      buildSuppIndex();
      return supp_index_;
    case S_NATIONKEY:
      buildNationIndex();
      return nation_index_;
    default:
      throw std::runtime_error("this column does not have key index.");
  }
}
std::shared_ptr<float_index_> TpchTableSupplier::FloatIndex(int col) {
  assert(col == S_ACCTBAL);
  buildAcctbalIndex();
  return acctbal_index_;
}

const std::vector<db_key_t_>::iterator TpchTableSupplier::KeyIterator(int col) {
  switch (col) {
    case S_SUPPKEY:
      return supp_key_.begin();
    case S_NATIONKEY:
      return nation_key_.begin();
    default:
      throw std::runtime_error("this column does not have an iterator for it.");
  }
}

void TpchTableSupplier::buildSuppIndex() {
  if (supp_index_ != nullptr) return;
  supp_index_.reset(new key_index_());
  db_key_t_ index = 0;
  for (db_key_t_ val : supp_key_) supp_index_->emplace(val, index++);
}

void TpchTableSupplier::buildNationIndex() {
  if (nation_index_ != nullptr) return;
  nation_index_.reset(new key_index_());
  db_key_t_ index = 0;
  for (db_key_t_ val : nation_key_) nation_index_->emplace(val, index++);
}
void TpchTableSupplier::buildAcctbalIndex() {
  if (acctbal_index_ != nullptr) return;
  acctbal_index_.reset(new float_index_());
  int64_t index = 0;
  for (float val : acctbal_) acctbal_index_->emplace(val, index++);
}
}  // namespace qjoin