#include "tpch_table_part.h"

namespace qjoin {
TpchTablePart::TpchTablePart(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  part_key_.resize(row_count);
  retail_price_.resize(row_count);
  size_.resize(row_count);

  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    size_t start = 0;
    size_t next = input_buffer.find(COL_DELIMITER, 0);

    part_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // skip name
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // skip mfgr
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // skip brand
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // skip type
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    size_[index] = strtol(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    // skip container
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    retail_price_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    ++index;
  }
  input_file.close();

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

DATABASE_DATA_TYPES TpchTablePart::GetColumnType(int col) const {
  switch (col) {
    case P_PARTKEY:
      return DATABASE_DATA_TYPES::INT64;
    case P_NAME:
    case P_TYPE:
    case P_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    case P_MFGR:
    case P_BRAND:
    case P_CONTAINER:
      return DATABASE_DATA_TYPES::CHAR;
    case P_SIZE:
      return DATABASE_DATA_TYPES::INT32;
    case P_RETAILPRICE:
      return DATABASE_DATA_TYPES::FLOAT;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTablePart::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == P_PARTKEY);
  return part_key_[row];
}

int32_t TpchTablePart::IntAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == P_SIZE);
  return size_[row];
}

float TpchTablePart::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == P_RETAILPRICE);
  return retail_price_[row];
}

std::shared_ptr<key_index_> TpchTablePart::KeyIndex(int col) {
  assert(col == P_PARTKEY);
  buildPartkeyIndex();
  return part_index_;
}

const std::vector<db_key_t_>::iterator TpchTablePart::KeyIterator(int col) {
  switch (col) {
    case P_PARTKEY:
      return part_key_.begin();
    default:
      throw std::runtime_error("there is no key index for such a column.");
  }
}

void TpchTablePart::buildPartkeyIndex() {
  if (part_index_ != nullptr) return;
  part_index_.reset(new key_index_());

  db_key_t_ index;
  for (db_key_t_ val : part_key_) part_index_->emplace(val, index++);
}
}  // namespace qjoin