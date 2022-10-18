#include "qjoin/table_impl.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "util/string_operators.h"

namespace qjoin {
// TableImpl::TableImpl() :
// col0_(std::make_shared<std::vector<db_key_t_>>()),col1_() {}
TableImpl::TableImpl(std::string filename, char delim, int col0, int col1) {
  table_name_ = filename;
  col_cnt_ = (col0 == col1) ? 1 : 2;
  col0_ = std::make_shared<std::vector<db_key_t_>>();
  col1_ = (col0 == col1) ? col0_ : std::make_shared<std::vector<db_key_t_>>();
  has_col2_ = false;

  FILE* fp = fopen(filename.c_str(), "r");
  assert(fp != nullptr);

  char* line = nullptr;
  size_t len = 0;
  row_count_ = 0;
  while (getline(&line, &len, fp) != -1) {
    // extract columns
    std::string buffer(line);
    if (col0 >= 0) {
      col0_->push_back(std::stoll(ParseColumnFromLine(buffer, delim, col0)));
    }

    if (col1 >= 0 && col0 != col1) {
      col1_->push_back(std::stoll(ParseColumnFromLine(buffer, delim, col1)));
    }

    row_count_++;
  }
  fclose(fp);
  if (line) free(line);
}

TableImpl::TableImpl(std::string filename, char delim, int col0, int col1,
                     int col2, DATABASE_DATA_TYPES col2_date_type) {
  // currently, the third column should only be  char*
  assert(col2_date_type == DATABASE_DATA_TYPES::CHAR);

  table_name_ = filename;
  col_cnt_ = (col0 == col1) ? 2 : 3;
  col0_ = std::make_shared<std::vector<db_key_t_>>();
  col1_ = (col0 == col1) ? col0_ : std::make_shared<std::vector<db_key_t_>>();
  col2_ = std::make_shared<std::vector<std::string>>();
  has_col2_ = true;

  FILE* fp = fopen(filename.c_str(), "r");
  assert(fp != nullptr);

  char* line = nullptr;
  size_t len = 0;
  row_count_ = 0;
  while (getline(&line, &len, fp) != -1) {
    // extract columns
    std::string buffer(line);
    if (col0 >= 0) {
      col0_->push_back(std::stoll(ParseColumnFromLine(buffer, delim, col0)));
    }

    if (col1 >= 0 && col0 != col1) {
      col1_->push_back(std::stoll(ParseColumnFromLine(buffer, delim, col1)));
    }

    if (col2 >= 0) {
      col2_->push_back(ParseColumnFromLine(buffer, delim, col2));
    }

    row_count_++;
  }
  fclose(fp);
  if (line) free(line);
}

TableImpl::~TableImpl() {}

db_key_t_ TableImpl::KeyValAt(int64_t row, int col) const {
  switch (col) {
    case 0:
      return col0_->at(row);
    case 1:
      return col1_->at(row);
    default:
      throw std::runtime_error("no such column.");
  }
}
std::string TableImpl::StrAt(int64_t row) const { return col2_->at(row); }

void TableImpl::BuildIndex() {
  buildKeyIndex();
  if (has_col2_) buildCharsIndex();
}

void TableImpl::buildKeyIndex() {
  if (col0_index_ != nullptr) return;
  // index for col0
  col0_index_ = std::make_shared<std::multimap<db_key_t_, int64_t>>();
  int64_t idx = 0;
  for (db_key_t_ val : *col0_) col0_index_->emplace(val, idx++);

  // index for col1
  if (col0_ == col1_) {
    col1_index_ = col0_index_;
  } else {
    col1_index_ = std::make_shared<std::multimap<db_key_t_, int64_t>>();
    int64_t idx = 0;
    for (db_key_t_ val : *col1_) col1_index_->emplace(val, idx++);
  }
}

void TableImpl::buildCharsIndex() {
  if (col2_index_ != nullptr) return;
  col2_index_ = std::make_shared<std::multimap<std::string, int64_t>>();
  int64_t idx = 0;
  for (std::string val : *col2_) col2_index_->emplace(val, idx++);
}

void TableImpl::buildKeyBloomFilter() {}
void TableImpl::buildCharsBloomFilter() {}
}  // namespace qjoin