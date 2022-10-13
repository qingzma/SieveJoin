#include "tpch_table_region.h"

#include <regex>

namespace qjoin {
TpchTableRegion::TpchTableRegion(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  region_key_.resize(row_count);
  name_.resize(row_count);

  // build the scanf string to use to read the file
  std::string formater = "(\\d+)\\";
  formater += COL_DELIMITER;
  formater += "([[:print:]]{0,25})\\";
  formater += COL_DELIMITER;
  formater += "([[:print:]]{0,152})\\";
  formater += COL_DELIMITER;
  std::smatch matcher;

  std::regex pattern(formater);

  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    if (regex_search(input_buffer, matcher, pattern)) {
      region_key_[index] = strtoll(matcher[1].str().c_str(), nullptr, 10);
      strcpy(name_[index].str, matcher[2].str().c_str());

      ++index;
      // discard the last field (no support for varchar!
    }
  }

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

DATABASE_DATA_TYPES TpchTableRegion::GetColumnType(int col) const {
  switch (col) {
    case R_REGIONKEY:
      return DATABASE_DATA_TYPES::INT64;
    case R_NAME:
      return DATABASE_DATA_TYPES::CHAR;
    case R_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTableRegion::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == R_REGIONKEY);
  return region_key_[row];
}

const char* TpchTableRegion::CharsAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == R_NAME);
  return name_[row].str;
}

std::shared_ptr<key_index_> TpchTableRegion::KeyIndex(int col) {
  assert(col == R_REGIONKEY);
  buildRegionIndex();
  return region_index_;
}

const std::vector<db_key_t_>::iterator TpchTableRegion::KeyIterator(int col) {
  assert(col == R_REGIONKEY);
  return region_key_.begin();
}

void TpchTableRegion::buildRegionIndex() {
  if (region_index_ != nullptr) return;
  region_index_.reset(new key_index_());
  db_key_t_ index = 0;
  for (db_key_t_ val : region_key_) region_index_->emplace(val, index++);
}
}  // namespace qjoin