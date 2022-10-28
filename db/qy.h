// QY - Suppliers that are in the same nation as a pair of customers
// in the same nation that has once ordered the same items:
//
// SELECT l1.l_linenumber, o1.o_orderkey, c1.c_custkey,
// l2.l_linenumber, o2.o_orderkey, s_suppkey,
// c2.c_custkey
// FROM lineitem l1, orders o1, customer c1,
// lineitem l2, orders o2, customer c2, supplier s
// WHERE l1.l_orderkey = o1.o_orderkey
// AND o1.o_custkey = c1.c_custkey
// AND l1.l_partkey = l2.l_partkey
// AND l2.l_orderkey = o2.o_orderkey
// AND o2.o_custkey = c2.c_custkey
// AND c1.c_nationkey = s.s_nationkey
// AND s.s_nationkey = c2.c_nationkey;

#ifndef QJOIN_DB_QX_H_
#define QJOIN_DB_QX_H_
#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class QueryY {
 private:
  Options options_;
  int64_t n_access_tuple_;
  int64_t n_access_index_;
  int64_t n_access_bf_;

  std::shared_ptr<TableImpl> tbl_nation_;
  std::shared_ptr<TableImpl> tbl_supplier_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_customer_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_orders_;    // 1, 0
  std::shared_ptr<TableImpl> tbl_lineitem_;

  void buildBloomFilter(int level);
  void resetCounter();

 public:
  QueryY(Options& options);
  void Run();
  void LoopJoin();
  void IndexJoin();
  void QLoopJoin();
  void QIndexJoin();
};

}  // namespace qjoin

#endif  // QJOIN_DB_QX_H_