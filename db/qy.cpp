#include "qy.h"

#include "table/tpch_schema_columns.h"
#include "util/timer.h"

namespace qjoin {
QueryY::~QueryY() {}

QueryY::QueryY(Options& options) {
  options_ = options;
  resetCounter();

  Timer timer;
  timer.Start();
  // load data
  tbl_lineitem_1_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "lineitem.tbl", '|', L_PARTKEY,
      L_ORDERKEY);  // 1, 0
  tbl_orders_1_ =
      std::make_shared<TableImpl>(options, options_.path_prefix + "orders.tbl",
                                  '|', O_ORDERKEY, O_CUSTKEY);  // 0, 1
  tbl_customer_1_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "customer.tbl", '|', C_CUSTKEY,
      C_NATIONKEY);  // 0, 3
  tbl_supplier_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "supplier.tbl", '|', S_NATIONKEY,
      S_SUPPKEY);  // 0, 3
  tbl_customer_2_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "customer.tbl", '|', C_NATIONKEY,
      C_CUSTKEY);  // 3, 0
  tbl_orders_2_ =
      std::make_shared<TableImpl>(options, options_.path_prefix + "orders.tbl",
                                  '|', O_CUSTKEY, O_ORDERKEY);  // 1, 0
  tbl_lineitem_2_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "lineitem.tbl", '|', L_ORDERKEY,
      L_PARTKEY);  // 1, 0
  std::cout << "time cost to load data: " << timer.Seconds() << " seconds."
            << std::endl;

  // build index
  timer.Mark();
  tbl_lineitem_1_->BuildIndex();
  tbl_orders_1_->BuildIndex();
  tbl_customer_1_->BuildIndex();
  tbl_supplier_->BuildIndex();
  tbl_customer_2_->BuildIndex();
  tbl_orders_2_->BuildIndex();
  tbl_lineitem_2_->BuildIndex();
  std::cout << "time cost to build index: " << timer.SecondsSinceMarked()
            << " seconds." << std::endl;

  if (options.q_loop_join || options.q_index_join) {
    timer.Mark();
    buildBloomFilter(0);
    std::cout << "time cost to build bloom filters: "
              << timer.SecondsSinceMarked() << " seconds." << std::endl;
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
  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void QueryY::QIndexJoin() {}
void QueryY::QLoopJoin() {}
void QueryY::IndexJoin() {}
void QueryY::LoopJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Loop join starts for query Y." << std::endl;
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream baseline_file(options_.path_prefix + "qx_loop.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;
  // loop lineitem1
  for (int64_t i0 = 0; i0 < tbl_lineitem_1_->Size(); i0++) {
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  baseline_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "Loop join ends for query Y with join size: " << join_cnt
            << std::endl;
}

void QueryY::buildBloomFilter(int level) {
  tbl_lineitem_1_->BuildKeyBloomFilter();
  tbl_orders_1_->BuildKeyBloomFilter();
  tbl_customer_1_->BuildKeyBloomFilter();
  tbl_supplier_->BuildKeyBloomFilter();
  tbl_customer_2_->BuildKeyBloomFilter();
  tbl_orders_2_->BuildKeyBloomFilter();
  tbl_lineitem_2_->BuildKeyBloomFilter();

  // merge bf from l2 to o2
  tbl_orders_2_->col1_bf_->UpdateBfFromOutsideColumn(
      tbl_orders_2_->col1_, *(tbl_lineitem_2_->col0_bf_));
  tbl_orders_2_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_orders_2_->col0_, tbl_orders_2_->col1_, *(tbl_orders_2_->col1_bf_));

  // merge bf from o2 to c2
  tbl_customer_2_->col1_bf_->UpdateBfFromOutsideColumn(
      tbl_customer_2_->col1_, *(tbl_orders_2_->col0_bf_));
  tbl_customer_2_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_customer_2_->col0_, tbl_customer_2_->col1_,
      *(tbl_customer_2_->col1_bf_));

  // merge bf from c2 to s
  tbl_supplier_->col0_bf_->UpdateBfFromOutsideColumn(
      tbl_supplier_->col0_, *(tbl_customer_2_->col0_bf_));

  // merge bf from s to c1
  tbl_customer_1_->col1_bf_->UpdateBfFromOutsideColumn(
      tbl_customer_1_->col1_, *(tbl_supplier_->col0_bf_));
  tbl_customer_1_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_customer_1_->col0_, tbl_customer_1_->col1_,
      *(tbl_supplier_->col1_bf_));

  // merge bf from c1 to o1
  tbl_orders_1_->col1_bf_->UpdateBfFromOutsideColumn(
      tbl_orders_1_->col1_, *(tbl_customer_1_->col0_bf_));
  tbl_orders_1_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_orders_1_->col0_, tbl_orders_1_->col1_, *(tbl_customer_1_->col0_bf_));

  // merge bf from o1 to l1
  tbl_lineitem_1_->col1_bf_->UpdateBfFromOutsideColumn(
      tbl_lineitem_1_->col1_, *(tbl_orders_1_->col0_bf_));
  tbl_lineitem_1_->col0_bf_->UpdateBfFromInsideColumnOutsideColumn(
      tbl_lineitem_1_->col0_, tbl_lineitem_1_->col1_,
      *(tbl_lineitem_1_->col1_bf_), *(tbl_lineitem_2_->col1_bf_));
}
}  // namespace qjoin