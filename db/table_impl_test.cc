#include "qjoin/table_impl.h"

#include <gtest/gtest.h>
#include <iostream>

#include "qjoin/options.h"

namespace qjoin {
Options options;
TEST(TableImpl, test_table_loading_one_column) {
  {
#ifdef HAVE_TPCH_01_CUSTOMER
    TableImpl tbl(options, HAVE_TPCH_01_CUSTOMER, '|', 0, 0);
    EXPECT_EQ(tbl.KeyValAt(0, 0), 1);
    EXPECT_EQ(tbl.KeyValAt(1, 0), 2);
#endif  // HAVE_TPCH_01_CUSTOMER
  }
}

TEST(TableImpl, test_table_loading_two_columns) {
#ifdef HAVE_TPCH_01_LINEITEM
  TableImpl tbl(options, HAVE_TPCH_01_LINEITEM, '|', 0, 1);
  EXPECT_EQ(tbl.KeyValAt(0, 0), 1);
  EXPECT_EQ(tbl.KeyValAt(1, 1), 6731);
#endif  // HAVE_TPCH_01_LINEITEM
}

TEST(TableImpl, test_table_loading_three_columns) {
#ifdef HAVE_TPCH_01_CUSTOMER
  TableImpl tbl(options, HAVE_TPCH_01_CUSTOMER, '|', 0, 0, 1,
                DATABASE_DATA_TYPES::CHAR);
  EXPECT_EQ(tbl.KeyValAt(0, 0), 1);
  EXPECT_STREQ(tbl.StrAt(0).c_str(), "Customer#000000001");
  EXPECT_STREQ(tbl.StrAt(14999).c_str(), "Customer#000015000");
#endif  // HAVE_TPCH_01_CUSTOMER
}

TEST(TableImpl, test_table_loading_three_columns_build_index) {
#ifdef HAVE_TPCH_01_CUSTOMER
  TableImpl tbl(options, HAVE_TPCH_01_CUSTOMER, '|', 0, 0, 1,
                DATABASE_DATA_TYPES::CHAR);
  tbl.BuildIndex();
  std::cout << tbl.col0_index_.get()->find(1)->second << std::endl;
  EXPECT_EQ(tbl.col0_index_.get()->find(1)->second, 0);
#endif  // HAVE_TPCH_01_CUSTOMER
}

}  // namespace qjoin
