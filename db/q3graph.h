// 3 clique graph query support
//
// SELECT t1.k1, t2.k1, t3.k1
// FROM t1, t2, t3,
// WHERE t1.k2 = t2.k1
// AND t2.k2 = t3.k1
// AND t3.k2 = t1.k1;

#ifndef QJOIN_DB_Q3GRAPH_H_
#define QJOIN_DB_Q3GRAPH_H_
#include "db/query.h"

#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class Query3Graph : public Query {
 private:
  std::shared_ptr<TableImpl> tbl1_;
  // std::shared_ptr<TableImpl> tbl2_;
  // std::shared_ptr<TableImpl> tbl3_;
  int64_t n_misses[4];

  void buildBloomFilter(int level) override;
  void resetCounter() override;

 public:
  Query3Graph(Options& options);
  ~Query3Graph();

  void Run() override;
  // void LoopJoin() override;
  void IndexJoin() override;
  // void QLoopJoin() override;
  void QIndexJoin() override;
  void QPlusIndexJoin() override;
};

}  // namespace qjoin

#endif  // QJOIN_DB_Q3GRAPH_H_