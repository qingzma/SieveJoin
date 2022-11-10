#include "qrst.h"

#include "util/prints.h"
#include "util/timer.h"
namespace qjoin {

QueryRst::~QueryRst() {}

void QueryRst::resetCounter() {
  n_access_tuple_ = 0;
  n_access_index_ = 0;
  n_access_bf_ = 0;
  memset(n_misses, 0, sizeof(n_misses));
}

QueryRst::QueryRst(Options& options) {
  // N_PRINT_GAP = 10000;
  options_ = options;
  resetCounter();
  Timer timer;
  timer.Start();

  // load data
  tbl_r_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "r" + options.path_suffix + ".csv", '|',
      0, 0);
  tbl_s_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "s" + options.path_suffix + ".csv", '|',
      0, 0);
  tbl_t_ = std::make_shared<TableImpl>(
      options, options_.path_prefix + "t" + options.path_suffix + ".csv", '|',
      0, 0);
  std::cout << "time cost to load data: " << timer.Seconds() << " seconds."
            << std::endl;

  // build indexes
  timer.Mark();
  tbl_r_->BuildIndex();
  tbl_s_->BuildIndex();
  tbl_t_->BuildIndex();
  std::cout << "time cost to build index: " << timer.SecondsSinceMarked()
            << " seconds." << std::endl;

  if (options.q_loop_join || options.q_index_join) {
    timer.Mark();
    buildBloomFilter(0);
    std::cout << "time cost to build bloom filters: "
              << timer.SecondsSinceMarked() << " seconds." << std::endl;
  }
}

void QueryRst::Run() {
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "running query synthetic R S T" << std::endl;
  Timer timer;
  timer.Start();

  if (options_.q_index_join) QIndexJoin();

  if (options_.q_loop_join) QLoopJoin();

  if (options_.index_join) IndexJoin();

  if (options_.loop_join) LoopJoin();

  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "done with query synthetic R S T" << std::endl;

  std::cout << "total time cost is " << timer.Seconds() << " seconds."
            << std::endl;
  std::cout << "********************************************" << std::endl;
  std::cout << "********************************************" << std::endl;
}

void QueryRst::IndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Index join starts for query rst." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream index_join_file(options_.path_prefix + "qrst_index.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;

  // loop r
  for (db_key_t_ r : *(tbl_r_->col0_)) {
    n_access_tuple_++;

    // check existance and loop  s
    n_access_index_++;
    auto s_ranges = tbl_s_->col0_index_->equal_range(r);
    for (auto s_iter = s_ranges.first; s_iter != s_ranges.second; s_iter++) {
      db_key_t_ s = s_iter->first;

      // check existance and loop t
      n_access_index_++;
      auto t_ranges = tbl_t_->col0_index_->equal_range(s);
      for (auto t_iter = t_ranges.first; t_iter != t_ranges.second; t_iter++) {
        db_key_t_ t = t_iter->first;
        join_cnt++;
        if (join_cnt % N_PRINT_GAP == 0) {
          std::cout << "\rfind " << join_cnt << " results" << std::flush;
        }
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
        index_join_file << r << "\n";
        if (join_cnt % N_PRINT_GAP == 0) index_join_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
      }
    }
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  index_join_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "\rtime cost: " << timer.Seconds() << " seconds." << std::endl;

  std::cout << "access tuples: " << n_access_tuple_ << std::endl;
  std::cout << "access indexed: " << n_access_index_ << std::endl;

  std::cout << "Index join ends for query rst with join size: " << join_cnt
            << std::endl;
}

void QueryRst::QIndexJoin() {
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "QIndexJoin starts for query x." << std::endl;
  resetCounter();
  Timer timer;
  timer.Start();

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream index_join_file(options_.path_prefix + "qrst_qindex_join.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "find 0 results" << std::flush;
  int64_t join_cnt = 0;

  // loop r
  for (db_key_t_ r : *(tbl_r_->col0_)) {
    n_access_tuple_++;

    // check existance and loop  s
    n_access_bf_++;
    if (!tbl_r_->col0_bf_->bf_.contains(r)) {
      n_misses[0]++;
      continue;
    }
    n_access_index_++;
    auto s_ranges = tbl_s_->col0_index_->equal_range(r);
    for (auto s_iter = s_ranges.first; s_iter != s_ranges.second; s_iter++) {
      db_key_t_ s = s_iter->first;

      // check existance and loop t
      n_access_bf_++;
      if (!tbl_s_->col0_bf_->bf_.contains(s)) {
        n_misses[1]++;
        continue;
      }
      n_access_index_++;
      auto t_ranges = tbl_t_->col0_index_->equal_range(s);
      for (auto t_iter = t_ranges.first; t_iter != t_ranges.second; t_iter++) {
        db_key_t_ t = t_iter->first;
        join_cnt++;
        if (join_cnt % N_PRINT_GAP == 0) {
          std::cout << "\rfind " << join_cnt << " results" << std::flush;
        }
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
        index_join_file << r << "\n";
        if (join_cnt % N_PRINT_GAP == 0) index_join_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
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
  print_n_misses(n_misses, 2);
  std::cout << "QIndexJoin ends for query x with join size: " << join_cnt
            << std::endl;
}

void QueryRst::buildBloomFilter(int level) {
  tbl_r_->BuildKeyBloomFilter();
  tbl_s_->BuildKeyBloomFilter();
  tbl_t_->BuildKeyBloomFilter();

  // merge bf from t to s
  tbl_s_->col0_bf_->UpdateBfFromOutsideColumn(tbl_s_->col0_,
                                              *(tbl_t_->col0_bf_));
  // merge bf from s and t to r
  tbl_r_->col0_bf_->UpdateBfFromOutsideColumns(
      tbl_r_->col0_, *(tbl_s_->col0_bf_), *(tbl_t_->col0_bf_));
}
}  // namespace qjoin