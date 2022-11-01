#include "qx.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>

#include "table/tpch_schema_columns.h"
#include "util/timer.h"

#include "qjoin/column_bloom_filter.h"
#include "qjoin/join_plan.h"
#include "qjoin/loop_join.h"
#include "qjoin/table_impl.h"

namespace qjoin {
QueryX::~QueryX() {}

QueryX::QueryX(Options& options) {
  options_ = options;
  resetCounter();
  // load data
  tbl_nation_ =
      std::make_shared<TableImpl>(options, options_.path_prefix + "nation.tbl",
                                  '|', N_NATION_KEY, N_NATION_KEY);
  tbl_supplier_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "supplier.tbl", '|', S_NATIONKEY,
      S_SUPPKEY);  // 3, 0
  tbl_customer_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "customer.tbl", '|', C_NATIONKEY,
      C_CUSTKEY);  // 3, 0
  tbl_orders_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "orders.tbl", '|', O_CUSTKEY,
      O_ORDERKEY);  // 1, 0
  tbl_lineitem_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "lineitem.tbl", '|', L_ORDERKEY,
      L_LINENUMBER);  // 0, 3

  // build indexes
  tbl_nation_->BuildIndex();
  tbl_supplier_->BuildIndex();
  tbl_customer_->BuildIndex();
  tbl_orders_->BuildIndex();
  tbl_lineitem_->BuildIndex();

  if (options.q_loop_join || options.q_index_join) {
    buildBloomFilter(0);
  }
}

void QueryX::Run() {
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "running query X" << std::endl;
  Timer timer;
  timer.Start();

  if (options_.q_index_join) QIndexJoin();

  if (options_.q_loop_join) QLoopJoin();

  if (options_.index_join) IndexJoin();

  if (options_.loop_join) LoopJoin();

  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "done with query X" << std::endl;

  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void QueryX::LoopJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Loop join starts for query x." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream baseline_file(options_.path_prefix + "qx_loop.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;
  // loop nation
  for (int64_t i0 = 0; i0 < tbl_nation_->Size(); i0++) {
    db_key_t_ n_nation = tbl_nation_->col0_->at(i0);

    // loop supplier
    for (int64_t i1 = 0; i1 < tbl_supplier_->Size(); i1++) {
      db_key_t_ s_nation = tbl_supplier_->col0_->at(i1);
      db_key_t_ s_supp = tbl_supplier_->col1_->at(i1);
      if (n_nation == s_nation) {
        // loop customer
        for (int64_t i2 = 0; i2 < tbl_customer_->Size(); i2++) {
          db_key_t_ c_nation = tbl_customer_->col0_->at(i2);
          db_key_t_ c_cust = tbl_customer_->col1_->at(i2);
          if (s_nation == c_nation) {
            // loop orders
            for (int64_t i3 = 0; i3 < tbl_orders_->Size(); i3++) {
              db_key_t_ o_cust = tbl_orders_->col0_->at(i3);
              db_key_t_ o_order = tbl_orders_->col1_->at(i3);
              if (c_cust == o_cust) {
                // loop lineitem
                for (int64_t i4 = 0; i4 < tbl_lineitem_->Size(); i4++) {
                  db_key_t_ l_order = tbl_lineitem_->col0_->at(i4);
                  db_key_t_ l_linenum = tbl_lineitem_->col1_->at(i4);
                  if (o_order == l_order) {
                    join_cnt++;
                    if (join_cnt % N_PRINT_GAP == 0) {
                      // if (join_cnt > 0) std::cout << '\r' << std::flush;
                      std::cout << "\rfind " << join_cnt << " results"
                                << std::flush;
                    }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
                    baseline_file << n_nation << "," << s_supp << "," << c_cust
                                  << "," << o_order << "," << l_linenum << "\n";
                    if (join_cnt % N_PRINT_GAP == 0) baseline_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
                  }
                }
              }
            }
          }
        }
      }
    }
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  baseline_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "Loop join ends for query x with join size: " << join_cnt
            << std::endl;
}

void QueryX::IndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Index join starts for query x." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream index_join_file(options_.path_prefix + "qx_index.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;
  // loop nation

  for (db_key_t_ n_nation : *(tbl_nation_->col0_)) {
    n_access_tuple_++;
    // check existance and loop supplier
    n_access_index_++;
    auto s_nation_iter = tbl_supplier_->col0_index_->equal_range(n_nation);
    for (auto iter_s = s_nation_iter.first; iter_s != s_nation_iter.second;
         iter_s++) {
      db_key_t_ s_nation = iter_s->first;
      int64_t s_idx = iter_s->second;
      n_access_tuple_++;
      db_key_t_ s_supp = tbl_supplier_->col1_->at(s_idx);
      // looop customer
      n_access_index_++;
      auto c_nation_iter = tbl_customer_->col0_index_->equal_range(s_nation);
      for (auto iter_c = c_nation_iter.first; iter_c != c_nation_iter.second;
           iter_c++) {
        db_key_t_ c_nation = iter_c->first;
        int64_t c_idx = iter_c->second;
        n_access_tuple_++;
        db_key_t_ c_cust = tbl_customer_->col1_->at(c_idx);
        // loop orders
        n_access_index_++;
        auto o_cust_iter = tbl_orders_->col0_index_->equal_range(c_cust);
        for (auto iter_o = o_cust_iter.first; iter_o != o_cust_iter.second;
             iter_o++) {
          db_key_t_ o_cust = iter_o->first;
          int64_t o_idx = iter_o->second;
          n_access_tuple_++;
          db_key_t_ o_order = tbl_orders_->col1_->at(o_idx);
          // loop lineitem
          n_access_index_++;
          auto l_order_iter = tbl_lineitem_->col0_index_->equal_range(o_order);
          for (auto iter_l = l_order_iter.first; iter_l != l_order_iter.second;
               iter_l++) {
            int64_t l_idx = iter_l->second;
            n_access_tuple_++;
            db_key_t_ l_linenum = tbl_lineitem_->col1_->at(l_idx);
            join_cnt++;
            if (join_cnt % N_PRINT_GAP == 0) {
              // if (join_cnt > 0) std::cout << '\r' << std::flush;
              std::cout << "\rfind " << join_cnt << " results" << std::flush;
            }
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
            index_join_file << n_nation << "," << s_supp << "," << c_cust << ","
                            << o_order << "," << l_linenum << "\n";
            if (join_cnt % N_PRINT_GAP == 0) index_join_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
          }
        }
      }
    }
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  index_join_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;

  std::cout << "access tuples: " << n_access_tuple_ << std::endl;
  std::cout << "access indexed: " << n_access_index_ << std::endl;

  std::cout << "Index join ends for query x with join size: " << join_cnt
            << std::endl;
}

void QueryX::QIndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "QIndexJoin starts for query x." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream index_join_file(options_.path_prefix + "qx_qindex_join.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;
  // loop nation

  for (db_key_t_ n_nation : *(tbl_nation_->col0_)) {
    n_access_tuple_++;
    // check existence and loop supplier
    n_access_bf_++;
    if (!tbl_nation_->col0_bf_->bf_.contains(n_nation))
      continue;  // skip if not exist in current table
    n_access_index_++;
    auto s_nation_iter = tbl_supplier_->col0_index_->equal_range(n_nation);
    for (auto iter_s = s_nation_iter.first; iter_s != s_nation_iter.second;
         iter_s++) {
      db_key_t_ s_nation = iter_s->first;
      int64_t s_idx = iter_s->second;
      n_access_tuple_++;
      db_key_t_ s_supp = tbl_supplier_->col1_->at(s_idx);
      // looop customer
      n_access_bf_++;
      if (!tbl_supplier_->col0_bf_->bf_.contains(s_nation))
        continue;  // skip if not exist in current table
      n_access_index_++;
      auto c_nation_iter = tbl_customer_->col0_index_->equal_range(s_nation);
      for (auto iter_c = c_nation_iter.first; iter_c != c_nation_iter.second;
           iter_c++) {
        db_key_t_ c_nation = iter_c->first;
        int64_t c_idx = iter_c->second;
        n_access_tuple_++;
        db_key_t_ c_cust = tbl_customer_->col1_->at(c_idx);
        // loop orders
        n_access_bf_++;
        if (!tbl_customer_->col1_bf_->bf_.contains(c_cust)) continue;
        n_access_index_++;
        auto o_cust_iter = tbl_orders_->col0_index_->equal_range(c_cust);
        for (auto iter_o = o_cust_iter.first; iter_o != o_cust_iter.second;
             iter_o++) {
          db_key_t_ o_cust = iter_o->first;
          int64_t o_idx = iter_o->second;
          n_access_tuple_++;
          db_key_t_ o_order = tbl_orders_->col1_->at(o_idx);
          n_access_bf_++;
          if (!tbl_orders_->col1_bf_->bf_.contains(o_order)) continue;
          // loop lineitem
          n_access_index_++;
          auto l_order_iter = tbl_lineitem_->col0_index_->equal_range(o_order);
          for (auto iter_l = l_order_iter.first; iter_l != l_order_iter.second;
               iter_l++) {
            int64_t l_idx = iter_l->second;
            n_access_tuple_++;
            db_key_t_ l_linenum = tbl_lineitem_->col1_->at(l_idx);
            join_cnt++;
            if (join_cnt % N_PRINT_GAP == 0) {
              // if (join_cnt > 0) std::cout << '\r' << std::flush;
              std::cout << "\rfind " << join_cnt << " results" << std::flush;
            }
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
            index_join_file << n_nation << "," << s_supp << "," << c_cust << ","
                            << o_order << "," << l_linenum << "\n";
            if (join_cnt % N_PRINT_GAP == 0) index_join_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
          }
        }
      }
    }
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  index_join_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "access tuples: " << n_access_tuple_ << std::endl;
  std::cout << "access indexed: " << n_access_index_ << std::endl;
  std::cout << "access bfs: " << n_access_bf_ << std::endl;

  std::cout << "QIndexJoin ends for query x with join size: " << join_cnt
            << std::endl;
}

void QueryX::QLoopJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "QLoopJoin starts for query x." << std::endl;
  std::cout << "QLoopJoin ends for query x." << std::endl;
}

void QueryX::buildBloomFilter(int level) {
  tbl_nation_->BuildKeyBloomFilter();
  tbl_supplier_->BuildKeyBloomFilter();
  tbl_customer_->BuildKeyBloomFilter();
  tbl_orders_->BuildKeyBloomFilter();
  tbl_lineitem_->BuildKeyBloomFilter();

  // merge bf from lineitem to order
  tbl_orders_->col1_bf_->UpdateBfFromOutsideColumn(tbl_orders_->col1_,
                                                   *(tbl_lineitem_->col0_bf_));
  tbl_orders_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_orders_->col0_, tbl_orders_->col1_, *(tbl_orders_->col1_bf_));

  // merge bf from order to customer
  tbl_customer_->col1_bf_->UpdateBfFromOutsideColumn(tbl_customer_->col1_,
                                                     *(tbl_orders_->col0_bf_));
  tbl_customer_->col0_bf_->UpdateBfFromInsideColumn(
      tbl_customer_->col0_, tbl_customer_->col1_, *(tbl_customer_->col1_bf_));

  // merge bf from customer to supplier
  tbl_supplier_->col0_bf_->UpdateBfFromOutsideColumn(
      tbl_supplier_->col0_, *(tbl_customer_->col0_bf_));

  // merge bf from supplier to nation
  tbl_nation_->col0_bf_->UpdateBfFromOutsideColumn(tbl_nation_->col0_,
                                                   *(tbl_supplier_->col0_bf_));
}

}  // namespace qjoin