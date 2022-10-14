#ifndef QJOIN_INCLUDE_H_
#define QJOIN_INCLUDE_H_
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "data_types.h"

namespace qjoin {
// declare index structure as multimap
typedef std::multimap<db_key_t_, int64_t> key_index_;
typedef std::multimap<int64_t, int64_t> int_index_;
typedef std::multimap<float, int64_t> float_index_;
class Table {
 public:
  Table() = default;
  Table(const Table&) = delete;
  // is not copyable
  Table& operator=(const Table&) = delete;
  virtual ~Table() = default;

  // row and colum count
  virtual int ColumnCount() const = 0;
  virtual int64_t RowCount() const = 0;
  virtual DATABASE_DATA_TYPES GetColumnType(int col) const = 0;

  // getter
  virtual int32_t IntAt(const int64_t& row, const int& col) const {
    throw std::runtime_error("not implemented.");
  };
  virtual int64_t Int64At(const int64_t& row, const int& col) const {
    throw std::runtime_error("not implemented.");
  };
  ;
  virtual float FloatAt(const int64_t& row, const int& col) const {
    throw std::runtime_error("not implemented.");
  };
  ;
  virtual const char* CharsAt(const int64_t& row, const int& col) const {
    throw std::runtime_error("not implemented.");
  };
  ;

  // index getters
  virtual std::shared_ptr<key_index_> KeyIndex(int col) = 0;
  std::shared_ptr<int_index_> IntIndex(int col) { return KeyIndex(col); };
  std::shared_ptr<float_index_> FloatIndex(int col);
  const std::vector<db_key_t_>::iterator KeyIterator(int col);
};
}  // namespace qjoin
#endif  // QJOIN_INCLUDE_H_