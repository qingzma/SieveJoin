#include "qx.h"

#include <iostream>
#include <memory>

#include "table/tpch_schema_columns.h"

#include "qjoin/table_impl.h"

namespace qjoin {
void Qx(Options& options) {
  std::cout << "running query X" << std::endl;

  // load data
  TableImpl tbl_nation_(options.path_prefix + "nation.tbl", '|', N_NATION_KEY,
                        N_NATION_KEY);
  TableImpl tbl_supplier_(options.path_prefix + "supplier.tbl", '|',
                          S_NATIONKEY, S_SUPPKEY);

  std::cout << "done with query X" << std::endl;
}
}  // namespace qjoin