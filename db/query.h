#ifndef QJOIN_DB_QUERY_H_
#define QJOIN_DB_QUERY_H_

#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class Query {
 protected:
  Options options_;
  int64_t n_access_tuple_;
  int64_t n_access_index_;
  int64_t n_access_bf_;
  int N_PRINT_GAP = 1000000;

  virtual void buildBloomFilter(int level);
  virtual void resetCounter();

 public:
  Query() = default;
  virtual ~Query();

  virtual void Run() = 0;
  virtual void LoopJoin();
  virtual void IndexJoin();
  virtual void QLoopJoin();
  virtual void QIndexJoin();
  virtual void QPlusIndexJoin();
};

}  // namespace qjoin

#endif  // QJOIN_DB_QUERY_H_