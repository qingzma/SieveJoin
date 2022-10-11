#ifndef QJOIN_INCLUDE_DATA_TYPE_H_
#define QJOIN_INCLUDE_DATA_TYPE_H_
#include <cstdint>
enum DATABASE_DATA_TYPES {
  INT32,
  INT64,
  FLOAT,
  CHAR,
  STRING,
  NONEXISTANT,
  LAST
};

constexpr const char COL_DELIMITER = '|';

typedef int64_t db_key_t_;
typedef int64_t db_time_t_;

#endif  // QJOIN_INCLUDE_DATA_TYPE_H_