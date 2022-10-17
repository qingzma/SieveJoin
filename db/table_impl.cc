#include "qjoin/table_impl.h"

#include <cassert>
#include <fstream>
#include <iostream>

namespace qjoin {
// TableImpl::TableImpl() :
// col0_(std::make_shared<std::vector<db_key_t_>>()),col1_() {}
TableImpl::TableImpl(std::string filename, char delim, int col0, int col1) {
  col0_ = std::make_shared<std::vector<db_key_t_>>();
  col1_ = (col0 == col1) ? col0_ : std::make_shared<std::vector<db_key_t_>>();
  table_name_ = filename;
  col_cnt_ = (col0 == col1) ? 1 : 2;

  FILE* fp = fopen(filename.c_str(), "r");
  assert(fp != nullptr);

  char* line = nullptr;
  size_t len = 0;
  while (getline(&line, &len, fp) != -1) {
    // extract columns
  }
  fclose(fp);
  if (line) free(line);
}

TableImpl::TableImpl(std::string filename, char delim, int col0, int col1,
                     int col3, DATABASE_DATA_TYPES col3_date_type) {}
TableImpl::~TableImpl() {}

void TableImpl::BuildIndex() {}

void TableImpl::buildKeyIndex() {}
void TableImpl::buildCharsIndex() {}

void TableImpl::buildKeyBloomFilter() {}
void TableImpl::buildCharsBloomFilter() {}
}  // namespace qjoin