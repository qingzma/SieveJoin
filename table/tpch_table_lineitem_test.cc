#include "tpch_table_lineitem.h"

#include <gtest/gtest.h>
#include <iostream>

namespace qjoin {
TEST(TpchTableLineitem, test_table_loading_lineitem) {
  // #ifdef HAVE_TPCH_01_LINEITEM
  //   TpchTableLineitem tbl(HAVE_TPCH_01_LINEITEM, 600572);
  //   EXPECT_EQ(tbl.Int64At(0, 0), 1);
  //   EXPECT_EQ(tbl.Int64At(1, 1), 6731);
  // #endif  // HAVE_TPCH_01_LINEITEM
}

}  // namespace qjoin
