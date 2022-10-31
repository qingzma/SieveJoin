#include "qy.h"

#include "table/tpch_schema_columns.h"
#include "util/timer.h"

namespace qjoin {
QueryY::~QueryY() {}

QueryY::QueryY(Options& options) {
  options_ = options;
  resetCounter();
  // load data
  tbl_lineitem_1_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "lineitem.tbl", '|', L_ORDERKEY,
      L_PARTKEY);
  // build index

  if (options.q_loop_join || options.q_index_join) {
    buildBloomFilter(0);
  }
}

void QueryY::Run() {
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "running query Y" << std::endl;
  Timer timer;
  timer.Start();

  if (options_.q_index_join) QIndexJoin();

  if (options_.q_loop_join) QLoopJoin();

  if (options_.index_join) IndexJoin();

  if (options_.loop_join) LoopJoin();

  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "done with query Y" << std::endl;
  timer.Stop();
  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void QueryY::QIndexJoin() {}
void QueryY::QLoopJoin() {}
void QueryY::IndexJoin() {}
void QueryY::LoopJoin() {}

void QueryY::buildBloomFilter(int level) {}
}  // namespace qjoin