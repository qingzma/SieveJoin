// QX - Suppliers and customers in the same nations with the
// purchase history of the customers.
//
// SELECT n_nationkey, s_suppkey, c_custkey,
// o_orderkey, l_linenumber  // 0, 1, 1, 1, 1
// FROM nation, supplier, customer,
// orders, lineitem
// WHERE n_nationkey = s_nationkey //0, 0
// AND s_nationkey = c_nationkey   //0, 0
// AND c_custkey = o_custkey       //1, 0
// AND o_orderkey = l_orderkey;    //1, 0

#ifndef QJOIN_DB_QX_H_
#define QJOIN_DB_QX_H_
#include "db/query.h"

#include "qjoin/options.h"
#include "qjoin/table_impl.h"

namespace qjoin {
class QueryX : public Query {
 private:
  std::shared_ptr<TableImpl> tbl_nation_;
  std::shared_ptr<TableImpl> tbl_supplier_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_customer_;  // 3, 0
  std::shared_ptr<TableImpl> tbl_orders_;    // 1, 0
  std::shared_ptr<TableImpl> tbl_lineitem_;
  int64_t n_misses[4];

  void buildBloomFilter(int level) override;
  void resetCounter() override;

 public:
  QueryX(Options& options);
  ~QueryX();

  void Run() override;
  void LoopJoin() override;
  void IndexJoin() override;
  void QLoopJoin() override;
  void QIndexJoin() override;
  void QPlusIndexJoin() override;
};

}  // namespace qjoin

#endif  // QJOIN_DB_QX_H_