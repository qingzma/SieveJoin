#include "tpch_table_supplier.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {

TEST(TpchTableSupplier, test_table_loading_supplier) {
#ifdef HAVE_TPCH_01_SUPPLIER
  TpchTableSupplier tbl(HAVE_TPCH_01_SUPPLIER, 1000);
  // std::cout << "haha" << tbl.Int64At(79999, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_SUPPLIER << std::endl;
  EXPECT_EQ(tbl.Int64At(999, 0), 1000);
  EXPECT_STREQ(tbl.CharsAt(999, 1), "Supplier#000001000");
#endif  // HAVE_TPCH_01_SUPPLIER
}

}  // namespace qjoin
