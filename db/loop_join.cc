#include "qjoin/loop_join.h"
namespace qjoin {
CyclicLoopJoin::CyclicLoopJoin() {}

void CyclicLoopJoin::Join(Options* options, JoinedTableLists& table_lists,
                          ColIsJoined& col_joined, ColIsJoined& cols_selected) {
#ifdef BOOL_WRITE_JOIN_RESULT_TO_FILE
  std::cout << "join result is saved to file." << std::endl;
#else
#endif  // BOOL_WRITE_JOIN_RESULT_TO_FILE
}
}  // namespace qjoin
