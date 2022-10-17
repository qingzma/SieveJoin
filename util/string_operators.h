#ifndef QJOIN_UTIL_STRING_OPERATORS_H_
#define QJOIN_UTIL_STRING_OPERATORS_H_
#include <string>

namespace qjoin {
std::string ParseColumnFromLine(std::string& buffer, const char& delim,
                                const int& col_id);
}

#endif  // QJOIN_UTIL_STRING_OPERATORS_H_