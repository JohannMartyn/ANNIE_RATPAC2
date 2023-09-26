#ifndef __RAT_GeoAnnieWbLSVolumeFactory__
#define __RAT_GeoAnnieWbLSVolumeFactory__

#include <GeoFactory.hh>

#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4OpticalSurface.hh>

namespace ANNIE {
    class GeoAnnieWbLSVolumeFactory : public GeoFactory {
        public:
            GeoAnnieWbLSVolumeFactory() : GeoFactory("WbLSVolume") {};
            virtual G4VPhysicalVolume *Construct(DBLinkPtr table);
        protected:
            G4VisAttributes *GetVisAttributes(DBLinkPtr table);
            G4OpticalSurface *GetSurface(std::string surface_name);
    };
  
} // namespace RAT

#endif
