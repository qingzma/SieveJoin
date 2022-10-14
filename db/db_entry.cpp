#include <iostream>

#include "qjoin/options.h"
#include "qx.h"

int main(int argc, char** argv) {
  qjoin::Options options;
  qjoin::ParseOptions(options, argc, argv);

  std::cout << "welcome to Quasi Join Demo" << std::endl;
  std::cout << "using scaling factor = " << options.scalingFactor << std::endl;

  if (options.qx) {
    qjoin::Qx();
  }
  return 0;
}
