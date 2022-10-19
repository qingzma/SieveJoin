#ifndef QJOIN_INCLUDE_OPTIONS_H
#define QJOIN_INCLUDE_OPTIONS_H

#include <string>

namespace qjoin {

struct Options {
  // float scalingFactor;
  bool qx;
  bool loop_join;   // run baseline loop join
  bool index_join;  // run index join
  bool qjoin_;      // run qjoin
  std::string path_prefix;
};

void ParseOptions(Options& options, int argc, char** argv);

}  // namespace qjoin

#endif