#include "tpch_table_lineitem.h"

namespace qjoin {

TpchTableLineitem::TpchTableLineitem(std::string filename, int64_t row_count)
    : table_name_(filename), row_count_(row_count) {
  order_key_.resize(row_count);
  part_key_.resize(row_count);
  supp_key_.resize(row_count);
  line_number_.resize(row_count);
  quantity_.resize(row_count);
  extended_price_.resize(row_count);
  discount_.resize(row_count);
  tax_.resize(row_count);
  return_flag_.resize(row_count);
  line_status_.resize(row_count);
  ship_date_.resize(row_count);
  commit_date_.resize(row_count);
  receipt_date_.resize(row_count);
  ship_instruct_.resize(row_count);
  ship_mode_.resize(row_count);

  time_t epoch = GetEpoch();

  struct tm time;
  time.tm_hour = 0;
  time.tm_isdst = 0;
  time.tm_min = 0;
  time.tm_sec = 0;
  int year, month, day;

  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);

  while (getline(input_file, input_buffer)) {
    size_t start = 0;
    size_t next = input_buffer.find(COL_DELIMITER, 0);

    order_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    part_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    supp_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    line_number_[index] = strtol(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    quantity_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    extended_price_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    discount_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    tax_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    return_flag_[index] = input_buffer[start];
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    line_status_[index] = input_buffer[start];
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    sscanf(&input_buffer[start], "%4i-%2i-%2i", &year, &month, &day);
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    ship_date_[index] = difftime(mktime(&time), epoch);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    sscanf(&input_buffer[start], "%4i-%2i-%2i", &year, &month, &day);
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    commit_date_[index] = difftime(mktime(&time), epoch);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    sscanf(&input_buffer[start], "%4i-%2i-%2i", &year, &month, &day);
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    receipt_date_[index] = difftime(mktime(&time), epoch);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(ship_instruct_[index].str, &input_buffer[start], next - start);
    ship_instruct_[index].str[next - start + 1] = 0;
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(ship_mode_[index].str, &input_buffer[start], next - start);
    ship_mode_[index].str[next - start + 1] = 0;
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    ++index;
  }
  input_file.close();

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

TpchTableLineitem::~TpchTableLineitem() {}

DATABASE_DATA_TYPES TpchTableLineitem::GetColumnType(int col) const {
  switch (col) {
    case L_ORDERKEY:
    case L_PARTKEY:
    case L_SUPPKEY:
      return DATABASE_DATA_TYPES::INT64;
    case L_LINENUMBER:
      return DATABASE_DATA_TYPES::INT32;
    case L_QUANTITY:
    case L_EXTENDEDPRICE:
    case L_DISCOUNT:
    case L_TAX:
      return DATABASE_DATA_TYPES::FLOAT;
    case L_RETURNFLAG:
    case L_LINESTATUS:
    case L_SHIPINSTRUCT:
    case L_SHIPMODE:
      return DATABASE_DATA_TYPES::CHAR;
    case L_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTableLineitem::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case L_ORDERKEY:
      return order_key_[row];
    case L_PARTKEY:
      return part_key_[row];
    case L_SUPPKEY:
      return supp_key_[row];
    case L_SHIPDATE:
      return ship_date_[row];
    case L_COMMITDATE:
      return commit_date_[row];
    case L_RECEIPTDATE:
      return receipt_date_[row];
    default:
      throw std::runtime_error("other columns are not stored.");
  }
}

int32_t TpchTableLineitem::IntAt(const int64_t& row, const int& col) const {
  assert((col == L_LINENUMBER) && (row < row_count_));
  return line_number_[row];
}

float TpchTableLineitem::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case L_QUANTITY:
      return quantity_[row];
    case L_EXTENDEDPRICE:
      return extended_price_[row];
    case L_DISCOUNT:
      return discount_[row];
    case L_TAX:
      return tax_[row];
    default:
      throw std::runtime_error("other columns are not stored.");
  }
}

const char* TpchTableLineitem::CharsAt(const int64_t& row,
                                       const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case L_RETURNFLAG:
      return &return_flag_[row];
    case L_LINESTATUS:
      return &line_status_[row];
    case L_SHIPINSTRUCT:
      return ship_instruct_[row].str;
    case L_SHIPMODE:
      return ship_mode_[row].str;

    default:
      throw std::runtime_error("other columns are not stored.");
  }
}

std::shared_ptr<key_index_> TpchTableLineitem::KeyIndex(int col) {
  switch (col) {
    case L_ORDERKEY:
      buildOrderIndex();
      return order_index_;
    case L_PARTKEY:
      buildPartIndex();
      return part_index_;
    case L_SUPPKEY:
      buildSuppIndex();
      return supp_index_;
    default:
      throw std::runtime_error("this column does not have key index.");
  }
}
std::shared_ptr<int_index_> TpchTableLineitem::IntIndex(int col) {
  switch (col) {
    case L_SHIPDATE:
      buildShipDateIndex();
      return ship_date_index;
    case L_COMMITDATE:
      buildCommitDateIndex();
      return commit_date_index;
    case L_RECEIPTDATE:
      buildReceiptDateIndex();
      return receipt_date_index;
    default:
      throw std::runtime_error("this column does not have int index.");
  }
}
const std::vector<db_key_t_>::iterator TpchTableLineitem::KeyIterator(int col) {
  switch (col) {
    case L_ORDERKEY:
      return order_key_.begin();
    case L_PARTKEY:
      return part_key_.begin();
    case L_SUPPKEY:
      return supp_key_.begin();
    case L_SHIPDATE:
      return ship_date_.begin();
    case L_COMMITDATE:
      return commit_date_.begin();
    case L_RECEIPTDATE:
      return receipt_date_.begin();
    default:
      throw std::runtime_error("this column does not have an iterator.");
  }
}

void TpchTableLineitem::buildOrderIndex() {
  if (order_index_ != nullptr) return;
  order_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : order_key_) order_index_->emplace(val, index++);
}
void TpchTableLineitem::buildPartIndex() {
  if (part_index_ != nullptr) return;
  part_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : part_key_) part_index_->emplace(val, index++);
}
void TpchTableLineitem::buildSuppIndex() {
  if (supp_index_ != nullptr) return;
  supp_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : supp_key_) supp_index_->emplace(val, index++);
}

void TpchTableLineitem::buildShipDateIndex() {
  if (ship_date_index != nullptr) return;
  ship_date_index.reset(new int_index_());
  int64_t index = 0;
  for (int64_t val : ship_date_) ship_date_index->emplace(val, index++);
}
void TpchTableLineitem::buildCommitDateIndex() {
  if (commit_date_index != nullptr) return;
  commit_date_index.reset(new int_index_());
  int64_t index = 0;
  for (int64_t val : commit_date_) commit_date_index->emplace(val, index++);
}
void TpchTableLineitem::buildReceiptDateIndex() {
  if (receipt_date_index != nullptr) return;
  receipt_date_index.reset(new int_index_());
  int64_t index = 0;
  for (int64_t val : receipt_date_) receipt_date_index->emplace(val, index++);
}

}  // namespace qjoin