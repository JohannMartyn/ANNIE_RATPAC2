#include <RAT/Rat.hh>
#include <RAT/AnyParse.hh>
#include <RAT/ProcBlockManager.hh>
#include <RAT/ProcAllocator.hh>
//#include <GeoANNIEFactory.hh>
#include <string>
#include <iostream>
#include <stdlib.h>

namespace annie {

class ANNIE : public RAT::Rat {
public:
  ANNIE(RAT::AnyParse* p, int argc, char** argv) : Rat(p, argc, argv) {
    // Append an additional data directory (for ratdb and geo)
    char* anniedata = getenv("ANNIEDATA");
    if (anniedata) {
      ratdb_directories.insert(std::string(anniedata) + "/ratdb");
    }

    //new GeoANNIEFactory();
  }
};

}  // namespace annie

int main(int argc, char** argv) {
  auto parser = new RAT::AnyParse(argc, argv);
  auto rat = annie::ANNIE(parser, argc, argv);
  rat.Begin();
  rat.Report();
}

