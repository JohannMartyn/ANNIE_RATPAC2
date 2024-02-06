#include <RAT/AnyParse.hh>
#include <RAT/Rat.hh>
#include <Annie.hh>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  auto parser = new RAT::AnyParse(argc, argv);
  auto annie = ANNIE::Annie(parser, argc, argv);
  annie.Begin();
  annie.Report();
}
