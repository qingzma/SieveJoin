#include "qjoin/table_impl.h"

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

  //   std::ifstream f;
  //   f.open(table_name_);
  //   std::str
  //   FILE * fp=fopen();
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