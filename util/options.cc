#include "qjoin/options.h"

namespace qjoin {

void ParseOptions(Options& options, int argc, char** argv) {
  TCLAP::CmdLine cmd("QJoin Arguments.");
  TCLAP::ValueArg<int> arg_SF("s", "sf", "saling factor", false, 10, "int");

  TCLAP::SwitchArg arg_QX("x", "qx", "do query x experiment", cmd, false);

  cmd.add(arg_SF);
  cmd.parse(argc, argv);

  options.scalingFactor = arg_SF.getValue();
  options.qx = arg_QX.getValue();
}
}  // namespace qjoin