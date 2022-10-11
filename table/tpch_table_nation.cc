#include "tpch_table_nation.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

#include "qjoin/data_types.h"

namespace qjoin {

TpchTableNation::TpchTableNation(std::string filename, size_t row_count)
    : table_name_(filename), row_count_(row_count) {
  nation_key_.resize(row_count);
  nation_name_.resize(row_count);
  region_key_.resize(row_count);

  // build the scanf string to use to read the file
  std::string formater = "(\\d+)\\";
  formater += COL_DELIMITER;

  formater += "([[:print:]]{0,25})\\";
  formater += COL_DELIMITER;

  formater += "(\\d+)\\";
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
      nation_key_[index] = strtoll(matcher[1].str().c_str(), nullptr, 10);
      strcpy(nation_name_[index].str, matcher[2].str().c_str());
      region_key_[index] = strtoll(matcher[3].str().c_str(), nullptr, 10);

      ++index;
      // discard the last field (no support for varchar!)
    }
  }

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

TpchTableNation::~TpchTableNation() {}

DATABASE_DATA_TYPES TpchTableNation::GetColumnType(int col) const {
  switch (col) {
    case 0:
      return DATABASE_DATA_TYPES::INT64;
    case 1:
      return DATABASE_DATA_TYPES::CHAR;
    case 2:
      return DATABASE_DATA_TYPES::INT64;
    case 3:
      return DATABASE_DATA_TYPES::STRING;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTableNation::Int64At(int64_t row, int col) const {
  switch (col) {
    case 0:
      return nation_key_[row];
    case 2:
      return region_key_[row];
    default:
      throw std::runtime_error("column type mismatch.");
  }
}
const char* TpchTableNation::CharsAt(int64_t row, int col) const {
  switch (col) {
    case 1:
      return nation_name_[row].str;
    default:
      throw std::runtime_error("column type mismatch.");
  }
}

const std::vector<db_key_t_>::iterator TpchTableNation::KeyIterator(int col) {
  switch (col) {
    case 0:

      return nation_key_.begin();
    case 2:

      return region_key_.begin();
    default:
      throw std::runtime_error("column type mismatch.");
  }
}

void TpchTableNation::build_nation_index() {
  if (nation_index_ != nullptr) return;

  nation_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ value : nation_key_) {
    nation_index_->emplace(value, index++);
  }
}

void TpchTableNation::build_region_index() {
  if (region_index_ != nullptr) return;

  region_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ value : region_key_) {
    region_index_->emplace(value, index++);
  }
}

std::shared_ptr<key_index_> TpchTableNation::KeyIndex(int col) {
  switch (col) {
    case 0:
      build_nation_index();
      return nation_index_;
    case 2:
      build_region_index();
      return region_index_;
    default:
      throw std::runtime_error("column type mismatch.");
  }
}

int TpchTableNation::ColumnCount() const { return col_count_; }

int64_t TpchTableNation::RowCount() const { return row_count_; }

}  // namespace qjoin