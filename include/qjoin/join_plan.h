#ifndef QJOIN_INCLUDE_JOIN_PLAN_H_
#define QJOIN_INCLUDE_JOIN_PLAN_H_
#include <vector>

#include "table_impl.h"
namespace qjoin {
typedef std::vector<std::shared_ptr<TableImpl>> JoinedTableLists;
typedef std::vector<std::vector<bool>> ColIsSelected;
typedef std::vector<std::vector<bool>> ColIsJoined;
}  // namespace qjoin
#endif  // QJOIN_INCLUDE_JOIN_PLAN_H_