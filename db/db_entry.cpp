#include <tclap/CmdLine.h>

#include <iostream>
struct Settings {
  int scalingFactor;
  bool do_QX;
};

Settings settings;

void QX();
void ParseSettings(int argc, char** argv);

int main(int argc, char** argv) {
  ParseSettings(argc, argv);

  std::cout << "Welcome to Quasi Join Demo" << std::endl;
  std::cout << "using scaling factor = " << settings.scalingFactor << std::endl;

  if (settings.do_QX) {
    std::cout << "running experiment x" << std::endl;
  }
  return 0;
}

void ParseSettings(int argc, char** argv) {
  TCLAP::CmdLine cmd("QJoin Arguments.");
  TCLAP::ValueArg<int> arg_SF("S", "SF", "saling factor", false, 10, "int");

  TCLAP::SwitchArg arg_doQX("", "do_QX", "do query x experiment", cmd, false);

  cmd.add(arg_SF);
  cmd.parse(argc, argv);

  settings.scalingFactor = arg_SF.getValue();
  settings.do_QX = arg_doQX.getValue();
}

void QX(){};