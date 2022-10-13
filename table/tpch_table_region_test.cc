#include "tpch_table_region.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {

TEST(TpchTableRegion, test_table_loading_region) {
#ifdef HAVE_TPCH_01_REGION
  TpchTableRegion tbl(HAVE_TPCH_01_REGION, 5);
  // std::cout << "haha" << tbl.Int64At(79999, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_REGION << std::endl;
  EXPECT_EQ(tbl.Int64At(4, 0), 4);
  EXPECT_STREQ(tbl.CharsAt(0, 1), "AFRICA");
#endif  // HAVE_TPCH_01_REGION
}

}  // namespace qjoin
