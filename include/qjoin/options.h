#ifndef QJOIN_INCLUDE_OPTIONS_H
#define QJOIN_INCLUDE_OPTIONS_H

#include <tclap/CmdLine.h>

namespace qjoin {

struct Options {
  int scalingFactor;
  bool qx;
};

void ParseOptions(Options& options, int argc, char** argv);

}  // namespace qjoin

#endif