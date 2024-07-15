#ifndef __ANNIE_Annie__
#define __ANNIE_Annie__

#include <Config.hh>
#include <RAT/AnyParse.hh>
#include <RAT/ProcAllocator.hh>
#include <RAT/ProcBlockManager.hh>
#include <RAT/Rat.hh>
#include <GeoANNIEFactory.hh>
#include <OutANNIEClusterProc.hh>

namespace ANNIE {
	class Annie : public RAT::Rat {
	public:
  	Annie(RAT::AnyParse *p, int argc, char **argv);
	};
} // namespace ANNIE

#endif // __ANNIE_Annie__
