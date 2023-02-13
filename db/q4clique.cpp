#include "q4clique.h"

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

int64_t QPlus4CliqueJoinPart(int n, int i, std::shared_ptr<TableImpl> tbl1) {
  // std::cout << "here1" << std::endl;
  int64_t join_cnt = 0;
  int64_t sz = tbl1->col0_bf_index_vec_->size();
  int64_t chunk = sz / n + 1;
  int64_t low_i = chunk * i;
  int64_t high_i = chunk * (i + 1);
  high_i = std::min(high_i, sz);

  // loop tbl1
  for (auto n_i = low_i; n_i != high_i; n_i++) {
    // std::cout << "here3" << tbl1->col0_bf_index_vec_->size() << ", " << n_i
    //           << std::endl;
    db_key_t_ t1c0 = tbl1->col0_bf_index_vec_->at(n_i);
    db_key_t_ t1c1 = tbl1->col1_bf_index_vec_->at(n_i);

    auto t2_ranges = tbl1->col0_2clique_bf_index_->equal_range(t1c1);
    for (auto iter2 = t2_ranges.first; iter2 != t2_ranges.second; iter2++) {
      int64_t t2_i = iter2->second;
      db_key_t_ t2c1 = tbl1->col1_->at(t2_i);
      // std::cout << "here4" << std::endl;

      auto t3_ranges = tbl1->col0_3clique_bf_index_->equal_range(t2c1);
      for (auto iter3 = t3_ranges.first; iter3 != t3_ranges.second; iter3++) {
        int64_t t3_i = iter3->second;
        // std::cout << tbl3->col1_->size() << std::endl;
        db_key_t_ t3c1 = tbl1->col1_->at(t3_i);

        auto t4_ranges = tbl1->col0_4clique_bf_index_->equal_range(t3c1);
        for (auto iter4 = t4_ranges.first; iter4 != t4_ranges.second; iter4++) {
          int64_t t4_i = iter4->second;
          db_key_t_ t4c1 = tbl1->col1_->at(t4_i);
          if (t4c1 == t1c0) {
            join_cnt++;
          }
        }
      }
    }
  }
  return join_cnt;
}

Query4Clique::~Query4Clique() {}

void Query4Clique::resetCounter() {
  n_access_tuple_ = 0;
  n_access_index_ = 0;
  n_access_bf_ = 0;
  memset(n_misses, 0, sizeof(n_misses));
}

Query4Clique::Query4Clique(Options& options) {
  options_ = options;
  resetCounter();
  Timer timer;
  timer.Start();

  char delim = (options_.delim == ',') ? ',' : '\t';

  // load data
  tbl1_ =
      std::make_shared<TableImpl>(options, options_.path_prefix, delim, 0, 1);
  std::cout << "time cost to load data: " << timer.Seconds() << " seconds."
            << std::endl;

  // build index
  timer.Mark();
  tbl1_->BuildIndex();
  std::cout << "time cost to build index: " << timer.SecondsSinceMarked()
            << " seconds." << std::endl;

  if (options.q_loop_join || options.q_index_join || options.qplus_index_join) {
    timer.Mark();
    buildBloomFilter(0);
    std::cout << "time cost to build bloom filters: "
              << timer.SecondsSinceMarked() << " seconds." << std::endl;
  }
}

void Query4Clique::Run() {
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "running 4-clique query..." << std::endl;
  Timer timer;
  timer.Start();

  if (options_.loop_join) LoopJoin();

  if (options_.index_join) IndexJoin();

  if (options_.q_loop_join) QLoopJoin();

  if (options_.q_index_join) QIndexJoin();

  if (options_.qplus_index_join) QPlusIndexJoin();

  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "done with 4-clique query" << std::endl;

  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void Query4Clique::IndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Index join starts for query x." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;

  // loop tbl1
  for (int64_t i1 = 0; i1 < tbl1_->col0_->size(); i1++) {
    db_key_t_ t1c0 = tbl1_->col0_->at(i1);
    db_key_t_ t1c1 = tbl1_->col1_->at(i1);
    auto t2_ranges = tbl1_->col0_index_->equal_range(t1c1);
    for (auto t2_iter = t2_ranges.first; t2_iter != t2_ranges.second;
         t2_iter++) {
      int64_t t2_i = t2_iter->second;
      db_key_t_ t2c1 = tbl1_->col1_->at(t2_i);
      auto t3_ranges = tbl1_->col0_index_->equal_range(t2c1);
      for (auto t3_iter = t3_ranges.first; t3_iter != t3_ranges.second;
           t3_iter++) {
        int64_t t3_i = t3_iter->second;
        db_key_t_ t3c1 = tbl1_->col1_->at(t3_i);
        auto t4_ranges = tbl1_->col0_index_->equal_range(t3c1);
        for (auto t4_iter = t4_ranges.first; t4_iter != t4_ranges.second;
             t4_iter++) {
          int64_t t4_i = t4_iter->second;
          db_key_t_ t4c1 = tbl1_->col1_->at(t4_i);
          if (t4c1 == t1c0) {
            join_cnt++;
            if (join_cnt % N_PRINT_GAP == 0) {
              std::cout << "\rfind " << join_cnt << " results" << std::flush;
            }
          }
        }
      }
    }
  }
  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "Index Join ends for 4 clique query  with join size: "
            << join_cnt << std::endl;
}

void Query4Clique::QIndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "QIndexJoin starts for 4-clique query ." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;

  for (int64_t t1_i = 0; t1_i < tbl1_->col0_->size(); t1_i++) {
    db_key_t_ t1c0 = tbl1_->col0_->at(t1_i);
    db_key_t_ t1c1 = tbl1_->col1_->at(t1_i);
    if (!tbl1_->col0_bf_->bf_.contains(t1c0) ||
        !tbl1_->col1_bf_->bf_.contains(t1c1)) {
      continue;
    }

    // loop tbl 2
    auto t2_ranges = tbl1_->col0_index_->equal_range(t1c1);
    for (auto iter2 = t2_ranges.first; iter2 != t2_ranges.second; iter2++) {
      int64_t i2 = iter2->second;
      db_key_t_ t2c0 = tbl1_->col0_->at(i2);
      db_key_t_ t2c1 = tbl1_->col1_->at(i2);
      if (!tbl1_->col0_2clique_bf_->bf_.contains(t2c0) ||
          !tbl1_->col1_2clique_bf_->bf_.contains(t2c1)) {
        continue;
      }
      // loop tbl3
      auto t3_ranges = tbl1_->col0_index_->equal_range(t2c1);
      for (auto iter3 = t3_ranges.first; iter3 != t3_ranges.second; iter3++) {
        int64_t i3 = iter3->second;
        db_key_t_ t3c0 = tbl1_->col0_->at(i3);
        db_key_t_ t3c1 = tbl1_->col1_->at(i3);
        if (!tbl1_->col0_3clique_bf_->bf_.contains(t3c0) ||
            !tbl1_->col1_3clique_bf_->bf_.contains(t3c1)) {
          continue;
        }
        // loop tbl4
        auto t4_ranges = tbl1_->col0_index_->equal_range(t3c1);
        for (auto iter4 = t4_ranges.first; iter4 != t4_ranges.second; iter4++) {
          int64_t i4 = iter4->second;
          db_key_t_ t4c1 = tbl1_->col1_->at(i4);
          if (t4c1 == t1c0) {
            join_cnt++;
            if (join_cnt % N_PRINT_GAP == 0) {
              std::cout << "\rfind " << join_cnt << " results" << std::flush;
            }
          }
        }
      }
    }
  }

  std::cout << "access tuples: " << n_access_tuple_ << std::endl;
  std::cout << "access indexed: " << n_access_index_ << std::endl;
  std::cout << "access bfs: " << n_access_bf_ << std::endl;
  print_n_misses(n_misses, 4);
  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "QIndexJoin ends for 4 clique query  with join size: "
            << join_cnt << std::endl;
}

void Query4Clique::QPlusIndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "QPlusIndexJoin starts for 4-clique query ." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

  int64_t join_cnt = 0;
  std::vector<std::future<int64_t>> tasks;
  int n = (options_.n_core == 0) ? std::thread::hardware_concurrency()
                                 : options_.n_core;
  for (int i = 0; i < n; i++) {
    // std::future<int64_t> task =
    //     std::async(QPlusJoinPart, 4, 0, tbl_r_, tbl_s_, tbl_t_);
    tasks.push_back(std::async(QPlus4CliqueJoinPart, n, i, tbl1_));
  }

  for (int i = 0; i < n; i++) {
    join_cnt += tasks[i].get();
  }

  std::cout << "time cost: " << timer.Seconds() << " seconds." << std::endl;
  std::cout << "QPlusIndexJoin ends for 4-clique query with join size: "
            << join_cnt << std::endl;
}

void Query4Clique::buildBloomFilter(int lvel) {
  tbl1_->BuildKeyBloomFilter(4);

  // merge bf to clique 2
  tbl1_->col0_2clique_bf_->UpdateBfFromOutsideColumn(tbl1_->col0_,
                                                     *(tbl1_->col1_bf_));
  tbl1_->col1_2clique_bf_->UpdateBfFromInsideColumn(tbl1_->col1_, tbl1_->col0_,
                                                    *(tbl1_->col0_2clique_bf_));

  // merge bf from clique 2 to clique 3
  tbl1_->col0_3clique_bf_->UpdateBfFromOutsideColumn(
      tbl1_->col0_, *(tbl1_->col1_2clique_bf_));
  tbl1_->col1_3clique_bf_->UpdateBfFromInsideColumn(tbl1_->col1_, tbl1_->col0_,
                                                    *(tbl1_->col0_3clique_bf_));

  // merge bf from clique 3 to clique 4
  tbl1_->col0_4clique_bf_->UpdateBfFromOutsideColumn(
      tbl1_->col0_, *(tbl1_->col1_3clique_bf_));
  tbl1_->col1_4clique_bf_->UpdateBfFromInsideColumnOutsideColumn(
      tbl1_->col1_, tbl1_->col0_, *(tbl1_->col0_4clique_bf_),
      *(tbl1_->col0_bf_));

  // QPlus join structure
  tbl1_->col0_2clique_bf_index_ =
      tbl1_->col0_2clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col0_, tbl1_->col1_2clique_bf_->bf_, tbl1_->col1_);
  tbl1_->col1_2clique_bf_index_ =
      tbl1_->col1_2clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col1_, tbl1_->col0_2clique_bf_->bf_, tbl1_->col0_);

  tbl1_->col0_3clique_bf_index_ =
      tbl1_->col0_3clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col0_, tbl1_->col1_3clique_bf_->bf_, tbl1_->col1_);
  tbl1_->col1_3clique_bf_index_ =
      tbl1_->col1_3clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col1_, tbl1_->col0_3clique_bf_->bf_, tbl1_->col0_);

  tbl1_->col0_4clique_bf_index_ =
      tbl1_->col0_4clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col0_, tbl1_->col1_4clique_bf_->bf_, tbl1_->col1_);
  tbl1_->col1_4clique_bf_index_ =
      tbl1_->col1_4clique_bf_->CreateBfIndexWithMultipleColumns(
          tbl1_->col1_, tbl1_->col0_4clique_bf_->bf_, tbl1_->col0_);

  tbl1_->col0_bf_index_ = tbl1_->col0_bf_->CreateBfIndexWithMultipleColumns(
      tbl1_->col0_, tbl1_->col1_bf_->bf_, tbl1_->col1_);
  tbl1_->col1_bf_index_ = tbl1_->col1_bf_->CreateBfIndexWithMultipleColumns(
      tbl1_->col1_, tbl1_->col0_bf_->bf_, tbl1_->col0_);

  //   tbl1_->BuildQPlusVecIndex();
  //   tbl1_->col0_bf_index_vec_ = tbl1_->col0_;
  //   tbl1_->col1_bf_index_vec_ = tbl1_->col1_;
  tbl1_->BuildQPlusVecIndex();
}

}  // namespace qjoin