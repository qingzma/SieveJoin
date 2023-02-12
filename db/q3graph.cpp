#include "q3graph.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

#include "table/tpch_schema_columns.h"
#include "util/prints.h"
#include "util/timer.h"

#include "qjoin/column_bloom_filter.h"
#include "qjoin/join_plan.h"
#include "qjoin/loop_join.h"
#include "qjoin/table_impl.h"

namespace qjoin {
Query3Graph::~Query3Graph() {}

void Query3Graph::resetCounter() {
  n_access_tuple_ = 0;
  n_access_index_ = 0;
  n_access_bf_ = 0;
  memset(n_misses, 0, sizeof(n_misses));
}

Query3Graph::Query3Graph(Options& options) {
  options_ = options;
  resetCounter();
  Timer timer;
  timer.Start();

  // load data
  tbl1_ = std::make_shared<TableImpl>(options, options_.path_prefix, ',', 0, 1);
  tbl2_ = std::make_shared<TableImpl>(options, options_.path_prefix, ',', 0, 1);
  tbl3_ = std::make_shared<TableImpl>(options, options_.path_prefix, ',', 0, 1);
  std::cout << "time cost to load data: " << timer.Seconds() << " seconds."
            << std::endl;

  // build index
  timer.Mark();
  tbl1_->BuildIndex();
  tbl2_->BuildIndex();
  tbl3_->BuildIndex();
  std::cout << "time cost to build index: " << timer.SecondsSinceMarked()
            << " seconds." << std::endl;

  if (options.q_loop_join || options.q_index_join || options.qplus_index_join) {
    timer.Mark();
    buildBloomFilter(0);
    std::cout << "time cost to build bloom filters: "
              << timer.SecondsSinceMarked() << " seconds." << std::endl;
  }
}

void Query3Graph::Run() {
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "running 3-clique query..." << std::endl;
  Timer timer;
  timer.Start();

  if (options_.q_index_join) QIndexJoin();

  if (options_.q_loop_join) QLoopJoin();

  if (options_.index_join) IndexJoin();

  if (options_.loop_join) LoopJoin();

  if (options_.qplus_index_join) QPlusIndexJoin();

  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "done with 3-clique query" << std::endl;

  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void Query3Graph::IndexJoin() {}

void Query3Graph::QIndexJoin() {}

void Query3Graph::QPlusIndexJoin() {}

void Query3Graph::buildBloomFilter(int lvel) {
  std::cout << "before bloom" << std::endl;
  tbl1_->BuildKeyBloomFilter();
  tbl2_->BuildKeyBloomFilter();
  tbl3_->BuildKeyBloomFilter();

  // merge bf from t3 to t2
  tbl3_->col0_bf_->UpdateBfFromInsideColumn(tbl3_->col0_, tbl3_->col1_,
                                            *(tbl3_->col1_bf_));
  tbl2_->col1_bf_->UpdateBfFromOutsideColumn(tbl2_->col1_, *(tbl3_->col0_bf_));
  tbl2_->col0_bf_->UpdateBfFromInsideColumn(tbl2_->col0_, tbl2_->col1_,
                                            *(tbl2_->col1_bf_));

  // merge bf from t2 to t1
  tbl1_->col1_bf_->UpdateBfFromOutsideColumn(tbl1_->col1_, *(tbl2_->col0_bf_));
  tbl1_->col0_bf_->UpdateBfFromInsideColumnOutsideColumn(
      tbl1_->col0_, tbl1_->col1_, *(tbl1_->col1_bf_), *(tbl3_->col1_bf_));

  // QPlus join structure
  tbl1_->col0_bf_index_ = tbl1_->col0_bf_->CreateBfIndexWithMultipleColumns(
      tbl1_->col0_, tbl1_->col1_bf_->bf_, tbl1_->col1_);
  tbl1_->col1_bf_index_ = tbl1_->col1_bf_->CreateBfIndexWithMultipleColumns(
      tbl1_->col1_, tbl1_->col0_bf_->bf_, tbl1_->col0_);

  tbl2_->col0_bf_index_ = tbl2_->col0_bf_->CreateBfIndexWithMultipleColumns(
      tbl2_->col0_, tbl2_->col1_bf_->bf_, tbl2_->col1_);
  tbl2_->col1_bf_index_ = tbl2_->col1_bf_->CreateBfIndexWithMultipleColumns(
      tbl2_->col1_, tbl2_->col0_bf_->bf_, tbl2_->col0_);

  tbl3_->col0_bf_index_ = tbl3_->col0_bf_->CreateBfIndexWithMultipleColumns(
      tbl3_->col0_, tbl3_->col1_bf_->bf_, tbl3_->col1_);
  tbl3_->col1_bf_index_ = tbl3_->col1_bf_->CreateBfIndexWithMultipleColumns(
      tbl3_->col1_, tbl3_->col0_bf_->bf_, tbl3_->col0_);
  tbl1_->col0_bf_index_vec_ = tbl1_->col0_bf_->CreateBfIndexVec(tbl1_->col0_);
}

}  // namespace qjoin