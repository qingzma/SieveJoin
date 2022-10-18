#ifndef QJOIN_INCLUDE_QJOIN_CYCLIC_JOIN_BASE_H_
#define QJOIN_INCLUDE_QJOIN_CYCLIC_JOIN_BASE_H_

#include "join_plan.h"
#include "options.h"
#include "table_impl.h"

namespace qjoin {
class CyclicLoopJoin {
 private:
 public:
  CyclicLoopJoin();
  void Join(Options* options, JoinedTableLists& table_lists,
            ColIsJoined& col_joined, ColIsJoined& cols_selected);
};
}  // namespace qjoin

#endif  // QJOIN_INCLUDE_QJOIN_CYCLIC_JOIN_BASE_H_