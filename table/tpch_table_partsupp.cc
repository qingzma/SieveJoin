#include "tpch_table_partsupp.h"
namespace qjoin {
TpchTablePartsupp::TpchTablePartsupp(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  part_key_.resize(row_count);
  supp_key_.resize(row_count);
  avail_qty_.resize(row_count);
  supply_cost_.resize(row_count);

  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    size_t start = 0;
    size_t next = input_buffer.find(COL_DELIMITER, 0);

    part_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    supp_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    avail_qty_[index] = strtol(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    supply_cost_[index] = strtof(&input_buffer[start], nullptr);

    ++index;
  }
  input_file.close();

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

DATABASE_DATA_TYPES TpchTablePartsupp::GetColumnType(int col) const {
  switch (col) {
    case PS_PARTKEY:
    case PS_SUPPKEY:
      return DATABASE_DATA_TYPES::INT64;
    case PS_AVAILQTY:
      return DATABASE_DATA_TYPES::INT32;
    case PS_SUPPLYCOST:
      return DATABASE_DATA_TYPES::FLOAT;
    case PS_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}
int64_t TpchTablePartsupp::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case PS_PARTKEY:
      return part_key_[row];
    case PS_SUPPKEY:
      return supp_key_[row];
    default:
      throw std::runtime_error("this column is unexpected.");
  }
}

float TpchTablePartsupp::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == PS_SUPPLYCOST);
  return supply_cost_[row];
}

int32_t TpchTablePartsupp::IntAt(const int64_t& row, const int& col) const {
  assert(col == PS_AVAILQTY);
  assert(row < row_count_);
  return avail_qty_[row];
}

std::shared_ptr<key_index_> TpchTablePartsupp::KeyIndex(int col) {
  switch (col) {
    case PS_PARTKEY:
      buildPartIndex();
      return part_index_;
    case PS_SUPPKEY:
      buildSuppIndex();
      return supp_index_;
    default:
      throw std::runtime_error("this column does not have key index");
  }
}
const std::vector<db_key_t_>::iterator TpchTablePartsupp::KeyIterator(int col) {
  switch (col) {
    case PS_PARTKEY:

      return part_key_.begin();
    case PS_SUPPKEY:

      return supp_key_.begin();
    default:
      throw std::runtime_error("this column does not have an iterator.");
  }
}
void TpchTablePartsupp::buildPartIndex() {
  if (part_index_ != nullptr) return;
  part_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : part_key_) part_index_->emplace(val, index++);
}
void TpchTablePartsupp::buildSuppIndex() {
  if (supp_index_ != nullptr) return;
  supp_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : supp_key_) supp_index_->emplace(val, index++);
}
}  // namespace qjoin