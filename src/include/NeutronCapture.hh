#ifndef __ANNIE_NeutronCapture__
#define __ANNIE_NeutronCapture__

// Local changes to neutron capture below 20 MeV (the Geant4 HP range), set up at /run/initialize.
//
// Isotope fix, for Geant4 >= 11.4.0 (still present in 11.5.0.beta): the default HP capture model
// nRadCaptureHP (G4NeutronRadCaptureHP) uses the target isotope picked by G4NeutronHPCaptureData,
// which weights isotopes by abundance only. The isotope inside the element Geant4 has chosen is
// re-picked by abundance x isotope cross section (G4NDL, the same data and weights as
// G4NeutronHPCapture, the default capture model up to Geant4 11.3).
//
// ANNRI-Gd: captures on 155Gd and 157Gd get their gamma cascade from the ANNRI-Gd model
// (src/ANNRIGd, tables in ratdb/ANNRIGd) instead of nRadCaptureHP, and ratpac-two's DICEBOX hook
// is skipped for them. All other captures stay with nRadCaptureHP.
// Condition of use set by the ANNRI-Gd authors: publications must cite both papers
// (full references in src/ANNRIGd/README.md):
//   K. Hagiwara et al. (ANNRI-Gd Collab.), PTEP 2019, 023D01, https://doi.org/10.1093/ptep/ptz002
//   T. Tanaka et al. (ANNRI-Gd Collab.), PTEP 2020, 043D02, https://doi.org/10.1093/ptep/ptaa015
//
// Macro commands, before /run/initialize:
//   /annie/physics/hpCaptureIsotopeFix true|false         (default: true)
//   /annie/physics/annriGd true|false                     (default: true)
//   /annie/physics/annriGdCascade full|discrete|continuum (default: full)

#include <G4GenericMessenger.hh>
#include <G4VStateDependent.hh>

namespace ANNIE {
class NeutronCapture : public G4VStateDependent {
 public:
  NeutronCapture();
  G4bool Notify(G4ApplicationState requestedState) override;

 private:
  G4bool fIsotopeFix = true;
  G4bool fANNRIGd = true;
  G4String fCascade = "full";
  G4bool fDone = false;
  G4GenericMessenger fMessenger;
};
}  // namespace ANNIE

#endif  // __ANNIE_NeutronCapture__
