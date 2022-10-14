#ifndef QJOIN_INCLUDE_QJOIN_COLUMN_EXTRACTOR_H_
#define QJOIN_INCLUDE_QJOIN_COLUMN_EXTRACTOR_H_

#include <algorithm>
#include <string>
#include <type_traits>

namespace qjoin {

struct datetime_t {};

template <typename T>
struct real_type {
  typedef T type;
};
// template <typename T>
// using real_type = T;

// Template specialization
template <>
struct real_type<datetime_t> {
  typedef time_t type;
};

template <typename T>
typename real_type<T>::type fromString(const std::string& line,
                                       std::string::size_type begin,
                                       std::string::size_type end);

// Template specialization
template <>
inline std::string fromString<std::string>(const std::string& line,
                                           std::string::size_type begin,
                                           std::string::size_type end) {
  return line.substr(begin, end - begin);
}

template <>
inline uint64_t fromString<uint64_t>(const std::string& line,
                                     std::string::size_type begin,
                                     std::string::size_type end) {
  return std::stoull(line.substr(begin, end - begin));
}

template <>
inline int fromString<int>(const std::string& line,
                           std::string::size_type begin,
                           std::string::size_type end) {
  return std::stoi(line.substr(begin, end - begin));
}

template <>
inline double fromString<double>(const std::string& line,
                                 std::string::size_type begin,
                                 std::string::size_type end) {
  return std::stod(line.substr(begin, end - begin));
}

template <>
inline time_t fromString<datetime_t>(const std::string& line,
                                     std::string::size_type begin,
                                     std::string::size_type end) {
  tm t;
  t.tm_sec = 0;
  t.tm_min = 0;
  t.tm_hour = 0;
  t.tm_isdst = -1;
  strptime(line.substr(begin, end - begin).c_str(), "%Y-%m-%d", &t);
  time_t t2 = mktime(&t);
  return t2 / (60ll * 60 * 24);
}

template <unsigned v>
using ColNo = std::integral_constant<unsigned, v>;

template <typename... T>
struct ColumnExtractorImpl {};

template <typename R, unsigned C, typename... T>
struct ColumnExtractorImpl<R, ColNo<C>, T...> {
  template <typename... U>
  auto operator()(char delim, const std::string& line, unsigned curcol,
                  std::string::size_type i, U... cols) const {
    while (curcol < C) {
      i = line.find(delim, i);
      i++;
      curcol++;
    }
    auto j = line.find(delim, i);
    if (j == std::string::npos) j = line.length();
    return ColumnExtractorImpl<T...>()(delim, line, curcol + 1, j + 1, cols...,
                                       fromString<R>(line, i, j));
  }
};

template <>
struct ColumnExtractorImpl<> {
  template <typename... U>
  std::tuple<U...> operator()(char delim, const std::string& line,
                              unsigned curcol, std::string::size_type i,
                              U... cols) const {
    return std::make_tuple(cols...);
  }
};

template <typename... T>
struct ColumnExtractor {
  auto operator()(char delim, const std::string& line) const {
    return ColumnExtractorImpl<T...>()(delim, line, 1u,
                                       (std::string::size_type)0);
  }
};

}  // namespace qjoin

#endif  // QJOIN_INCLUDE_QJOIN_COLUMN_EXTRACTOR_H_