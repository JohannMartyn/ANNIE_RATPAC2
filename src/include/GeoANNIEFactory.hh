#ifndef __RAT_GeoANNIEFactory__
#define __RAT_GeoANNIEFactory__

#include <RAT/GeoFactory.hh>

#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4OpticalSurface.hh>

namespace RAT {
    class GeoANNIEFactory : public GeoFactory {
        public:
            GeoANNIEFactory() : GeoFactory("annieInnerStructures") {};
            virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
        protected:
            void ConstructANNIEHolders(G4LogicalVolume *motherLog, const G4String& file_name, const std::vector<double> &color, const G4int invisible);
            void ConstructLUXETELHolders(G4LogicalVolume *motherLog, const G4String& file_name, const std::vector<double> &color, const G4int invisible, const G4int sandi_yes);
            void ConstructBlackSheet(G4LogicalVolume *motherLog, const G4String& file_name, const std::vector<double> &color, const G4int invisible);
    };
  
} // namespace RAT

#endif
