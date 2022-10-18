#ifndef QJOIN_INCLUDE_OPTIONS_H
#define QJOIN_INCLUDE_OPTIONS_H

#include <tclap/CmdLine.h>

namespace qjoin {

struct Options {
  // float scalingFactor;
  bool qx;
  std::string path_prefix;
};

void ParseOptions(Options& options, int argc, char** argv);

}  // namespace qjoin

#endif