#include "tpch_table_nation.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {
TEST(TpchTableNation, test_table_loading_nation) {
#ifdef HAVE_TPCH_01_NATION
  TpchTableNation tbl(HAVE_TPCH_01_NATION, 25);
  // std::cout << tbl.Int64At(24, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_NATION << std::endl;
  EXPECT_EQ(tbl.Int64At(0, 0), 0);
  EXPECT_STREQ(tbl.CharsAt(0, 1), "ALGERIA");
#endif  // HAVE_TPCH_01_NATION
}

}  // namespace qjoin
