#include <fstream>
#include <iostream>
#include <string>

void create_table(char table, int d, int64_t r);

int main(int argc, char* argv[]) {
  std::cout << "generating R S T synthetic tables..." << std::endl;

  if (argc < 3) {
    std::cout << "usage is: rst r d" << std::endl;
    std::cout << "for instance rst 10000 1" << std::endl;
    return 1;
  }

  create_table('r', std::stoi(argv[1]), std::stoll(argv[2]));
  create_table('s', std::stoi(argv[1]), std::stoll(argv[2]));
  create_table('t', std::stoi(argv[1]), std::stoll(argv[2]));
}

void create_table(char table, int d, int64_t r) {
  if (table != 'r' && table != 's' && table != 't') {
    std::cout << "unexpected table name" << std::endl;
    return;
  }

  std::string ch(1, table);
  std::string filename =
      ch + "_" + std::to_string(d) + "_" + std::to_string(r) + ".csv";
  std::ofstream f(filename);
  if (f.is_open()) {
    int64_t lower = 1, upper = 10000000;
    if (table == 's') upper = (upper + r) / 2;
    if (table == 't') lower = (upper - r) / 2;
    for (int64_t i = lower; i <= upper; i++) {
      for (int j = 0; j < d; j++) {
        f << i << "\n";
      }
    }
  } else {
    std::cout << "unable to open file" << std::endl;
  }
}