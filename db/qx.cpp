#include "qx.h"

#include <iostream>
#include <memory>

#include "table/tpch_schema_columns.h"

#include "qjoin/join_plan.h"
#include "qjoin/loop_join.h"
#include "qjoin/table_impl.h"

namespace qjoin {
void Qx(Options& options) {
  std::cout << "running query X" << std::endl;

  // load data
  std::shared_ptr<TableImpl> tbl_nation_(
      new TableImpl(options.path_prefix + "nation.tbl", '|', N_NATION_KEY,
                    N_NATION_KEY));  // 0, 0
  std::shared_ptr<TableImpl> tbl_supplier_(
      new TableImpl(options.path_prefix + "supplier.tbl", '|', S_NATIONKEY,
                    S_SUPPKEY));  // 3, 0
  std::shared_ptr<TableImpl> tbl_customer_(
      new TableImpl(options.path_prefix + "customer.tbl", '|', C_NATIONKEY,
                    C_CUSTKEY));  // 3, 0
  std::shared_ptr<TableImpl> tbl_orders_(
      new TableImpl(options.path_prefix + "orders.tbl", '|', O_CUSTKEY,
                    O_ORDERKEY));  // 1, 0
  std::shared_ptr<TableImpl> tbl_lineitem_(
      new TableImpl(options.path_prefix + "lineitem.tbl", '|', L_ORDERKEY,
                    L_LINENUMBER));  // 0, 3

  // build indexes
  tbl_nation_->BuildIndex();
  tbl_supplier_->BuildIndex();
  tbl_customer_->BuildIndex();
  tbl_orders_->BuildIndex();
  tbl_lineitem_->BuildIndex();

  //   JoinedTableLists table_lists = {tbl_nation_, tbl_supplier_,
  //   tbl_customer_,
  //                                   tbl_orders_, tbl_lineitem_};

  //   // create the join plan
  //   ColIsSelected colIsSelected = {};
  //   ColIsJoined colIsJoined = {};
  //   // table nation
  //   colIsSelected.emplace_back(std::vector<bool>{true, false});
  //   colIsJoined.emplace_back(std::vector<bool>{true, false});
  //   // table supplier
  //   colIsSelected.emplace_back(std::vector<bool>{false, true});
  //   colIsJoined.emplace_back(std::vector<bool>{true, false});
  //   // table customer
  //   colIsSelected.emplace_back(std::vector<bool>{false, true});
  //   colIsJoined.emplace_back(std::vector<bool>{true, true});
  //   // table orders
  //   colIsSelected.emplace_back(std::vector<bool>{false, true});
  //   colIsJoined.emplace_back(std::vector<bool>{true, true});
  //   // table lineitem
  //   colIsSelected.emplace_back(std::vector<bool>{false, true});
  //   colIsJoined.emplace_back(std::vector<bool>{true, false});

  //   CyclicLoopJoin loop_join;
  //   loop_join.Join(&options, table_lists, colIsJoined, colIsSelected);

  for (auto iter = tbl_nation_->KeyIterator(0);
       iter < tbl_nation_->KeyIterator(0) + tbl_nation_.)
    std::cout << "done with query X" << std::endl;
}
}  // namespace qjoin