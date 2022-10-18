#ifndef QJOIN_INCLUDE_JOIN_PLAN_H_
#define QJOIN_INCLUDE_JOIN_PLAN_H_
#include <vector>
namespace qjoin {
typedef std::vector<std::vector<bool>> ColIsSelected;
typedef std::vector<std::vector<bool>> ColIsJoined;
}  // namespace qjoin
#endif  // QJOIN_INCLUDE_JOIN_PLAN_H_