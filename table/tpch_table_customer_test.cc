#include "tpch_table_customer.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {
TEST(TpchTableCustomer, test_table_loading_customer) {
#ifdef HAVE_TPCH_01_CUSTOMER
  TpchTableCustomer tbl(HAVE_TPCH_01_CUSTOMER, 15000);
  // std::cout << tbl.Int64At(24, 0) << std::endl;
  //   std::cout << tbl.CharsAt(14999, 4) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_CUSTOMER << std::endl;
  EXPECT_EQ(tbl.Int64At(0, 0), 1);
  EXPECT_STREQ(tbl.CharsAt(14999, 4), "13-684-946-3096");
#endif  // HAVE_TPCH_01_CUSTOMER
}

}  // namespace qjoin
