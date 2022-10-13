#include "tpch_table_part.h"

#include <gtest/gtest.h>

#include <iostream>

namespace qjoin {

TEST(TpchTablePart, test_table_loading_part) {
#ifdef HAVE_TPCH_01_PART
  TpchTablePart tbl(HAVE_TPCH_01_PART, 20000);
  // std::cout << tbl.Int64At(0, 0) << std::endl;
  // std::cout << tbl.CharsAt(0, 1) << std::endl;
  // std::cout << "table found " << HAVE_TPCH_01_PART << std::endl;
  EXPECT_EQ(tbl.Int64At(19999, 0), 20000);
  EXPECT_EQ(tbl.Int64At(0, 0), 1);
#endif  // HAVE_TPCH_01_PART
}

}  // namespace qjoin
