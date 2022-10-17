#include "string_operators.h"

namespace qjoin {
std::string ParseColumnFromLine(std::string& buffer, const char& delim,
                                const int& col_id) {
  size_t idx = 0;
  for (int i = 0; i < col_id; i++) {
    idx = buffer.find(delim, idx);
    idx++;
  }
  size_t idx_r = buffer.find(delim, idx);
  return buffer.substr(idx, idx_r - idx);
}

}  // namespace qjoin