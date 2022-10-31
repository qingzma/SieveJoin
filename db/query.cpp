#include "query.h"

namespace qjoin {
void Query::resetCounter() {
  n_access_tuple_ = 0;
  n_access_index_ = 0;
  n_access_bf_ = 0;
}

void Query::buildBloomFilter(int level) {
  throw std::runtime_error("not implemented.");
}

// void Query::Run() { throw std::runtime_error("not implemented."); }
void Query::LoopJoin() { throw std::runtime_error("not implemented."); }
void Query::IndexJoin() { throw std::runtime_error("not implemented."); }
void Query::QLoopJoin() { throw std::runtime_error("not implemented."); }
void Query::QIndexJoin() { throw std::runtime_error("not implemented."); }
}  // namespace qjoin