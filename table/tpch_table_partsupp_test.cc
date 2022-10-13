#include "tpch_table_partsupp.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {

TEST(TpchTablePartsupp, test_table_loading_partsupp) {
#ifdef HAVE_TPCH_01_PARTSUPP
  TpchTablePartsupp tbl(HAVE_TPCH_01_PARTSUPP, 80000);
  // std::cout << "haha" << tbl.Int64At(79999, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_PARTSUPP << std::endl;
  EXPECT_EQ(tbl.Int64At(79999, 0), 20000);
  EXPECT_EQ(tbl.Int64At(0, 0), 1);
#endif  // HAVE_TPCH_01_PARTSUPP
}

}  // namespace qjoin
