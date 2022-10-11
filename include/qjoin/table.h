#ifndef QJOIN_INCLUDE_H_
#define QJOIN_INCLUDE_H_
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "data_types.h"

// declare index structure as multimap
typedef std::multimap<db_key_t_, int64_t> key_index_;
typedef std::multimap<int64_t, int64_t> int_index_;
typedef std::multimap<float, int64_t> float_index_;
class Table {
 private:
 public:
  Table() = default;
  Table(const Table&) = delete;
  // is not copyable
  Table& operator=(const Table&) = delete;
  virtual ~Table();

  // row and colum count
  virtual int ColumnCount() const = 0;
  virtual int64_t RowCount() const = 0;
  virtual DATABASE_DATA_TYPES GetColumnType(int& col) const = 0;

  // getter
  virtual int32_t IntAt(int64_t row, int col) const;
  virtual int64_t Int64At(int64_t row, int col) const;
  virtual float FloatAt(int64_t row, int col) const;
  virtual const char* CharsAt(int64_t row, int col) const;

  // index getters
  virtual std::shared_ptr<key_index_> KeyIndex(int col) const = 0;
  std::shared_ptr<int_index_> IntIndex(int col) const { return KeyIndex(col); };
  virtual std::shared_ptr<float_index_> FloatIndex(int col) const = 0;
  const std::vector<db_key_t_>::iterator KeyIterator(int col) {
    return std::vector<db_key_t_>::iterator();
  };
};

#endif  // QJOIN_INCLUDE_H_