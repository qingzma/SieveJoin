#include "qjoin/loop_join.h"

#include <cassert>
namespace qjoin {
CyclicLoopJoin::CyclicLoopJoin() {}

void CyclicLoopJoin::Join(Options* options, JoinedTableLists& table_lists,
                          ColIsJoined& col_joined, ColIsJoined& cols_selected) {
  assert(table_lists.size() == col_joined.size() &&
         col_joined.size() == cols_selected.size());
  int n_table = table_lists.size();

  for (int i = 0; i < n_table; i++) {
    auto tbl = table_lists[i];
    bool access_c0 = col_joined[i][0] || cols_selected[i][0];
    bool access_c1 = col_joined[i][1] || cols_selected[i][1];

    if (!access_c0 || !access_c1)
      throw std::runtime_error("this column is not required to be accessed: " +
                               std::to_string(i) + "th table.");
  }

#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "join result is saved to file." << std::endl;
#else
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
}
}  // namespace qjoin
