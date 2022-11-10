#include <iostream>

#include "qjoin/options.h"
#include "qrst.h"
#include "qx.h"
#include "qy.h"

int main(int argc, char** argv) {
  qjoin::Options options;
  qjoin::ParseOptions(options, argc, argv);

  std::cout << "welcome to Quasi Join Demo" << std::endl;
  std::cout << "data is located at " << options.path_prefix << std::endl;

  if (options.qx) {
    qjoin::QueryX qx(options);
    qx.Run();
  }

  if (options.qy) {
    qjoin::QueryY qy(options);
    qy.Run();
  }

  if (options.qrst) {
    qjoin::QueryRst qrst(options);
    qrst.Run();
  }
  return 0;
}
