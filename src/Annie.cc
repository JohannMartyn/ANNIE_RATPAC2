#include <Annie.hh>

namespace ANNIE {
Annie::Annie(RAT::AnyParse *p, int argc, char **argv)
  : Rat(p, argc, argv) {
    // Append an additional data directory (for ratdb and geo)
    char *anniedata = getenv("ANNIEDATA");
    if (anniedata != NULL) {
      ratdb_directories.insert(static_cast<std::string>(anniedata) + "/ratdb");
      model_directories.insert(static_cast<std::string>(anniedata) + "/models");
  }
  // Initialize a geometry factory
  new RAT::GeoANNIEFactory();
  // Include a new type of processor
  RAT::ProcBlockManager::AppendProcessor<RAT::OutANNIEClusterProc>();
  //procAllocators["outanniecluster"] = new ProcAllocatorTmpl<OutANNIEClusterProc>;
  // Add a unique component to the datastructure
}
} // namespace ANNIE
