#include "qjoin/options.h"

#include <tclap/CmdLine.h>
namespace qjoin {

void ParseOptions(Options& options, int argc, char** argv) {
  TCLAP::CmdLine cmd("QJoin Arguments.");
  TCLAP::ValueArg<float> arg_SF("s", "sf", "saling factor", false, 10, "float");
  TCLAP::ValueArg<std::string> arg_PATH("p", "path", "data path", false,
                                        "../data/tpch/x0.1/", "string");
  TCLAP::ValueArg<std::string> arg_SKEW("k", "skew", "skew data extra string",
                                        false, "", "string");

  TCLAP::SwitchArg arg_QX("x", "qx", "do query x experiment", cmd, false);
  TCLAP::SwitchArg arg_QY("y", "qy", "do query y experiment", cmd, false);
  TCLAP::SwitchArg arg_QRST("", "qrst", "do query synthetic R S T experiment",
                            cmd, false);
  TCLAP::SwitchArg arg_LOOP_JOIN("l", "loop", "do  loop join", cmd, false);
  TCLAP::SwitchArg arg_INDEX_JOIN("i", "index", "do  index join", cmd, false);
  TCLAP::SwitchArg arg_QJOIN_INDEX("q", "qindexjoin", "do  qjoinindex", cmd,
                                   false);
  TCLAP::SwitchArg arg_QJOIN_LOOP("", "qloopjoin", "do  qjoinloop", cmd, false);

  cmd.add(arg_SF);
  cmd.add(arg_PATH);
  cmd.add(arg_SKEW);
  cmd.parse(argc, argv);

  // options.scalingFactor = arg_SF.getValue();
  options.qx = arg_QX.getValue();
  options.qy = arg_QY.getValue();
  options.qrst = arg_QRST.getValue();
  options.path_prefix = arg_PATH.getValue();
  options.skew_prefix = arg_SKEW.getValue();
  options.loop_join = arg_LOOP_JOIN.getValue();       // run baseline loop join
  options.index_join = arg_INDEX_JOIN.getValue();     // run index join
  options.q_index_join = arg_QJOIN_INDEX.getValue();  // run qjoin index
  options.q_loop_join = arg_QJOIN_LOOP.getValue();    // run qjoin loop
}
}  // namespace qjoin