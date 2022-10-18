#include "qjoin/options.h"

namespace qjoin {

void ParseOptions(Options& options, int argc, char** argv) {
  TCLAP::CmdLine cmd("QJoin Arguments.");
  TCLAP::ValueArg<float> arg_SF("s", "sf", "saling factor", false, 10, "float");
  TCLAP::ValueArg<std::string> arg_PATH("p", "path", "data path", false,
                                        "../data/tpch/x0.1/", "string");

  TCLAP::SwitchArg arg_QX("x", "qx", "do query x experiment", cmd, false);

  cmd.add(arg_SF);
  cmd.add(arg_PATH);
  cmd.parse(argc, argv);

  // options.scalingFactor = arg_SF.getValue();
  options.qx = arg_QX.getValue();
  options.path_prefix = arg_PATH.getValue();
}
}  // namespace qjoin