#include "qx.h"

#include <fstream>
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
  //
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

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::ofstream baseline_file(options.path_prefix + "qx_baseline.txt");
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  int64_t join_cnt = 0;
  // loop nation
  for (int64_t i0 = 0; i0 < tbl_nation_->Size(); i0++) {
    db_key_t_ n_nation = tbl_nation_->col0_->at(i0);

    // loop supplier
    for (int64_t i1 = 0; i1 < tbl_supplier_->Size(); i1++) {
      db_key_t_ s_nation = tbl_supplier_->col0_->at(i1);
      db_key_t_ s_supp = tbl_supplier_->col1_->at(i1);
      if (n_nation == s_nation) {
        // loop customer
        for (int64_t i2 = 0; i2 < tbl_customer_->Size(); i2++) {
          db_key_t_ c_nation = tbl_customer_->col0_->at(i2);
          db_key_t_ c_cust = tbl_customer_->col1_->at(i2);
          if (s_nation == c_nation) {
            // loop orders
            for (int64_t i3 = 0; i3 < tbl_orders_->Size(); i3++) {
              db_key_t_ o_cust = tbl_orders_->col0_->at(i3);
              db_key_t_ o_order = tbl_orders_->col1_->at(i3);
              if (c_cust == o_cust) {
                // loop lineitem
                for (int64_t i4 = 0; i4 < tbl_lineitem_->Size(); i4++) {
                  db_key_t_ l_order = tbl_lineitem_->col0_->at(i4);
                  db_key_t_ l_linenum = tbl_lineitem_->col1_->at(i4);
                  if (o_order == l_order) {
                    join_cnt++;
                    if (join_cnt % 10000 == 0)
                      std::cout << "find " << join_cnt << " results"
                                << std::endl;
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
                    baseline_file << n_nation << "," << s_supp << "," << c_cust
                                  << "," << o_order << "," << l_linenum << "\n";
                    if (join_cnt % 10000 == 0) baseline_file.flush();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
                  }
                }
              }
            }
          }
        }
      }
    }
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  baseline_file.close();
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE

  std::cout << "join size is " << join_cnt << std::endl;
  std::cout << "done with query X" << std::endl;
}
}  // namespace qjoin