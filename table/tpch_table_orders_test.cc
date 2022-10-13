#include "tpch_table_orders.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {

TEST(TpchTableOrders, test_table_loading_orders) {
#ifdef HAVE_TPCH_01_ORDERS
  TpchTableOrders tbl(HAVE_TPCH_01_ORDERS, 150000);
  // std::cout << tbl.Int64At(0, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_ORDERS << std::endl;
  EXPECT_EQ(tbl.Int64At(0, 1), 3691);
  EXPECT_STREQ(tbl.CharsAt(0, 6), "Clerk#000000951");
#endif  // HAVE_TPCH_01_ORDERS
}

}  // namespace qjoin
