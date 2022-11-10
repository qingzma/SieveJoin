// SELECT COUNT(*)
// FROM r,
//      s,
//      t
// WHERE r.attr = s.attr
//   AND r.attr = t.attr
//   AND s.attr = t.attr;
#ifndef QJOIN_DB_QRST_H_
#define QJOIN_DB_QRST_H_
#include "db/query.h"

#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class QueryRst : public Query {
 private:
  std::shared_ptr<TableImpl> tbl_r_;
  std::shared_ptr<TableImpl> tbl_s_;
  std::shared_ptr<TableImpl> tbl_t_;
  int64_t n_misses[2];

  void buildBloomFilter(int level) override;
  void resetCounter() override;

  // int64_t QPlusJoinPart(int n, int i);

 public:
  QueryRst(Options& options);
  ~QueryRst();

  void Run() override;
  // void LoopJoin() override;
  void IndexJoin() override;
  // void QLoopJoin() override;
  void QIndexJoin() override;
  void QPlusIndexJoin() override;
};
}  // namespace qjoin
#endif  // QJOIN_DB_QRST_H_