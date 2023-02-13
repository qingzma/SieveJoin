#include "qjoin/table_impl.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "util/string_operators.h"

namespace qjoin {
// TableImpl::TableImpl() :
// col0_(std::make_shared<std::vector<db_key_t_>>()),col1_() {}
TableImpl::TableImpl(Options& options, std::string filename, char delim,
                     int col0, int col1) {
  table_name_ = filename;
  col_cnt_ = (col0 == col1) ? 1 : 2;
  col0_ = std::make_shared<std::vector<db_key_t_>>();
  col1_ = (col0 == col1) ? col0_ : std::make_shared<std::vector<db_key_t_>>();
  has_col2_ = false;
  options_ = &options;

  FILE* fp = fopen(filename.c_str(), "r");
  if (fp == nullptr) {
    std::cout << "table file not found: " << filename << std::endl;
    throw std::runtime_error("table file not found: " + filename);
  }

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

TableImpl::TableImpl(Options& options, std::string filename, char delim,
                     int col0, int col1, int col2,
                     DATABASE_DATA_TYPES col2_date_type) {
  // currently, the third column should only be  char*
  assert(col2_date_type == DATABASE_DATA_TYPES::CHAR);

  table_name_ = filename;
  col_cnt_ = (col0 == col1) ? 2 : 3;
  col0_ = std::make_shared<std::vector<db_key_t_>>();
  col1_ = (col0 == col1) ? col0_ : std::make_shared<std::vector<db_key_t_>>();
  col2_ = std::make_shared<std::vector<std::string>>();
  has_col2_ = true;
  options_ = &options;

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

std::vector<db_key_t_>::iterator TableImpl::KeyIterator(int col) const {
  switch (col) {
    case 0:
      return col0_->begin();
    case 1:
      return col1_->begin();
    default:
      throw std::runtime_error("unexpected column to get key iterator.");
  }
}

void TableImpl::BuildKeyBloomFilter(int clique) {
  // NOTE, for 4+ clique queries, please add code below.
  // for cases where clique=1
  col0_bf_ = std::make_shared<ColumnBloomFilter>(*options_, row_count_);
  col0_bf_->Fit(col0_);

  if (col0_ != col1_) {
    col1_bf_ = std::make_shared<ColumnBloomFilter>(*options_, row_count_);
    col1_bf_->Fit(col1_);
  } else
    col1_bf_ = col0_bf_;

  // process 2 clique queries
  if (clique > 1) {
    col0_2clique_bf_ =
        std::make_shared<ColumnBloomFilter>(*options_, row_count_);
    col0_2clique_bf_->Fit(col0_);

    if (col0_ != col1_) {
      col1_2clique_bf_ =
          std::make_shared<ColumnBloomFilter>(*options_, row_count_);
      col1_2clique_bf_->Fit(col1_);
    } else
      col1_2clique_bf_ = col0_2clique_bf_;
  }

  // process 3 clique queries
  if (clique > 2) {
    col0_3clique_bf_ =
        std::make_shared<ColumnBloomFilter>(*options_, row_count_);
    col0_3clique_bf_->Fit(col0_);

    if (col0_ != col1_) {
      col1_3clique_bf_ =
          std::make_shared<ColumnBloomFilter>(*options_, row_count_);
      col1_3clique_bf_->Fit(col1_);
    } else
      col1_3clique_bf_ = col0_3clique_bf_;
  }

  // process 4 clique queries
  if (clique > 3) {
    col0_4clique_bf_ =
        std::make_shared<ColumnBloomFilter>(*options_, row_count_);
    col0_4clique_bf_->Fit(col0_);

    if (col0_ != col1_) {
      col1_4clique_bf_ =
          std::make_shared<ColumnBloomFilter>(*options_, row_count_);
      col1_4clique_bf_->Fit(col1_);
    } else
      col1_4clique_bf_ = col0_4clique_bf_;
  }
}
void TableImpl::BuildCharsBloomFilter() {}

void TableImpl::BuildQPlusVecIndex() {
  col0_bf_index_vec_ = std::make_shared<std::vector<db_key_t_>>();
  if (col1_) col1_bf_index_vec_ = std::make_shared<std::vector<db_key_t_>>();

  // std::shared_ptr<ColumnBloomFilter> c0bf, c1bf;

  // if (col0_4clique_bf_) {
  //   c0bf = col0_4clique_bf_;
  //   if (col1_) c1bf = col1_4clique_bf_;
  // } else if (col0_3clique_bf_) {
  //   c0bf = col0_3clique_bf_;
  //   if (col1_) c1bf = col1_3clique_bf_;
  // } else if (col0_2clique_bf_) {
  //   c0bf = col0_2clique_bf_;
  //   if (col1_) c1bf = col1_2clique_bf_;
  // } else {
  //   c0bf = col0_bf_;
  //   if (col1_) c1bf = col1_bf_;
  // }

  for (int64_t i = 0; i < col0_->size(); i++) {
    if (col1_) {
      if (col0_bf_->bf_.contains(col0_->at(i)) &&
          col1_bf_->bf_.contains(col1_->at(i))) {
        col0_bf_index_vec_->emplace_back(col0_->at(i));
        col1_bf_index_vec_->emplace_back(col1_->at(i));
      }
    } else {
      if (col0_bf_->bf_.contains(col0_->at(i))) {
        col0_bf_index_vec_->emplace_back(col0_->at(i));
      }
    }
  }
}
}  // namespace qjoin