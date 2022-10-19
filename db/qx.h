// QX - Suppliers and customers in the same nations with the
// purchase history of the customers.
//
// SELECT n_nationkey, s_suppkey, c_custkey,
// o_orderkey, l_linenumber
// FROM nation, supplier, customer,
// orders, lineitem
// WHERE n_nationkey = s_nationkey
// AND s_nationkey = c_nationkey
// AND c_custkey = o_custkey
// AND o_orderkey = l_orderkey;

#ifndef QJOIN_DB_QX_H_
#define QJOIN_DB_QX_H_
#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class QueryX {
 private:
  Options options_;

  std::shared_ptr<TableImpl> tbl_nation_;
  std::shared_ptr<TableImpl> tbl_supplier_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_customer_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_orders_;    // 1, 0
  std::shared_ptr<TableImpl> tbl_lineitem_;

 public:
  QueryX(Options& options);
  void Run();
  void LoopJoin();
  void IndexJoin();
  void QJoin();
};

}  // namespace qjoin

#endif  // QJOIN_DB_QX_H_