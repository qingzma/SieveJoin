#include "prints.h"

#include <iostream>

namespace qjoin {
void print_n_misses(int64_t* n_misses, int size) {
  std::cout << "misses statistics: " << std::endl;
  for (int i = 0; i < size; i++) {
    std::cout << "level " << i << ":\t" << n_misses[i] << std::endl;
  }
  std::cout << std::flush;
}
}  // namespace qjoin