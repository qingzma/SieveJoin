// 4 clique graph query support
//
// SELECT t1.k1, t2.k1, t3.k1, t4.k1
// FROM t1, t2, t3, t4
// WHERE t1.k2 = t2.k1
// AND t2.k2 = t3.k1
// AND t3.k2 = t4.k1
// AND t4.k2= t1.k1

#ifndef QJOIN_DB_Q4CLIQUE_H_
#define QJOIN_DB_Q4CLIQUE_H_
#include "db/query.h"

#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class Query4Clique : public Query {
 private:
  std::shared_ptr<TableImpl> tbl1_;
  int64_t n_misses[4];

  void buildBloomFilter(int level) override;
  void resetCounter() override;
  void buildTinyIndex();

 public:
  Query4Clique(Options& options);
  ~Query4Clique();

  void Run() override;
  // void LoopJoin() override;
  void IndexJoin() override;
  // void QLoopJoin() override;
  void QIndexJoin() override;
  void QPlusIndexJoin() override;
  void QTinyIndexJoin();
};
}  // namespace qjoin

#endif  // QJOIN_DB_Q4CLIQUE_H_