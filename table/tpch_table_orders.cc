#include "tpch_table_orders.h"

#include "util/Timer.h"
namespace qjoin {

TpchTableOrders::TpchTableOrders(std::string filename, int64_t row_count)
    : table_name_(filename), row_count_(row_count) {
  order_key_.resize(row_count);
  cust_key_.resize(row_count);
  order_status_.resize(row_count);
  total_price_.resize(row_count);
  order_date_.resize(row_count);
  order_priority_.resize(row_count);
  clerk_.resize(row_count);
  ship_priority_.resize(row_count);

  time_t epoch = GetEpoch();
  size_t index = 0;
  std::string input_buffer;
  std::ifstream input_file(filename);
  while (getline(input_file, input_buffer)) {
    size_t start = 0;
    size_t next = input_buffer.find(COL_DELIMITER, 0);

    order_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    cust_key_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    order_status_[index] = input_buffer[start];
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    total_price_[index] = strtof(&input_buffer[start], nullptr);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    struct tm time;
    int year, month, day;
    memset(&time, 0, sizeof(time));
    sscanf(&input_buffer[start], "%4i-%2i-%2i", &year, &month, &day);
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    order_date_[index] = difftime(mktime(&time), epoch);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(order_priority_[index].str, &input_buffer[start], next - start);
    order_priority_[index].str[next - start + 1] = 0;
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    strncpy(clerk_[index].str, &input_buffer[start], next - start);
    clerk_[index].str[next - start + 1] = 0;
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    ship_priority_[index] = strtoll(&input_buffer[start], nullptr, 10);
    start = next + 1;
    next = input_buffer.find(COL_DELIMITER, start);

    ++index;
  }
  input_file.close();

  if (index != row_count)
    std::cerr << "The table is a different size than it was reported to expect";
}

DATABASE_DATA_TYPES TpchTableOrders::GetColumnType(int col) const {
  switch (col) {
    case O_ORDERKEY:
    case O_CUSTKEY:
    case O_CLERK:
      return DATABASE_DATA_TYPES::INT64;
    case O_ORDERSTATUS:
    case O_ORDERDATE:
    case O_ORDERPRIORITY:
      return DATABASE_DATA_TYPES::CHAR;
    case O_TOTALPRICE:
      return DATABASE_DATA_TYPES::FLOAT;
    case O_SHIPPRIORITY:
      return DATABASE_DATA_TYPES::INT32;
    case O_COMMENT:
      return DATABASE_DATA_TYPES::STRING;
    default:
      return DATABASE_DATA_TYPES::NONEXISTANT;
  }
}

int64_t TpchTableOrders::Int64At(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case O_ORDERKEY:
      return order_key_[row];
    case O_CUSTKEY:
      return cust_key_[row];
    case O_ORDERDATE:
      return order_date_[row];
    default:
      throw std::runtime_error("fetching such a column is not supported.");
  }
}

float TpchTableOrders::FloatAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  assert(col == O_TOTALPRICE);
  return total_price_[row];
}

const char* TpchTableOrders::CharsAt(const int64_t& row, const int& col) const {
  assert(row < row_count_);
  switch (col) {
    case O_ORDERSTATUS:
      return &order_status_[row];
    case O_ORDERPRIORITY:
      return order_priority_[row].str;
    case O_CLERK:
      return clerk_[row].str;
    default:
      throw std::runtime_error("fetching such a column is not supported.");
  }
}

std::shared_ptr<key_index_> TpchTableOrders::KeyIndex(int col) {
  switch (col) {
    case O_ORDERKEY:
      buildOrdersIndex();
      return order_index_;
    case O_CUSTKEY:
      buildCustIndex();
      return cust_index_;
    default:
      throw std::runtime_error("this column does not have key index.");
  }
}
std::shared_ptr<int_index_> TpchTableOrders::IntIndex(int col) {
  switch (col) {
    case O_ORDERDATE:
      buildOrderdateIndex();
      return order_date_index;
    default:
      return KeyIndex(col);
  }
}
const std::vector<db_key_t_>::iterator TpchTableOrders::KeyIterator(int col) {
  switch (col) {
    case O_ORDERKEY:
      return order_key_.begin();
    case O_CUSTKEY:
      return cust_key_.begin();
    case O_ORDERDATE:
      return order_date_.begin();
    default:
      throw std::runtime_error("this column does not have an iterator.");
  }
}

void TpchTableOrders::buildOrdersIndex() {
  if (order_index_ != nullptr) return;
  order_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : order_key_) order_index_->emplace(val, index++);
}
void TpchTableOrders::buildCustIndex() {
  if (cust_index_ != nullptr) return;
  cust_index_.reset(new key_index_());

  db_key_t_ index = 0;
  for (db_key_t_ val : cust_key_) cust_index_->emplace(val, index++);
}
void TpchTableOrders::buildOrderdateIndex() {
  if (order_date_index != nullptr) return;
  order_date_index.reset(new int_index_());

  int64_t index = 0;
  for (int64_t val : order_date_) order_date_index->emplace(val, index++);
}
}  // namespace qjoin