// Auto-generated companion header for members.cc -- DO NOT EDIT BY HAND.
// Declares the ANNIE Phase-2 inner-structure builders.
#ifndef ANNIE_INNER_STRUCTURE_ANNIEINNERSTRUCTUREMEMBERS_HH
#define ANNIE_INNER_STRUCTURE_ANNIEINNERSTRUCTUREMEMBERS_HH

#include "G4String.hh"
#include <vector>

class G4MultiUnion;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;
class G4VPhysicalVolume;

G4MultiUnion* BuildANNIEInnerStructureSolid(const G4String& name);      // nested
G4MultiUnion* BuildANNIEInnerStructureSolidFlat(const G4String& name);  // 2199 flat nodes
G4LogicalVolume* MakeANNIEInnerStructureLV(G4Material*, G4OpticalSurface*, bool nested = true);

// VISUALISATION PATH: one placement per member, so each is a plain G4Box /
// G4SubtractionSolid that polyhedronises instantly.  G4MultiUnion CANNOT be
// visualised at this size -- CreatePolyhedron() queues 357 sequential CLHEP
// polyhedron unions and does not finish.
//
// structRot / structCentre are the SAME values you pass to the G4PVPlacement of
// the whole structure (rotm, StructureCenter).  They are composed with each
// member's own transform here; pass nullptr for no rotation.
// Reports the 226 member-member overlaps -- expected, harmless for viewing.
std::vector<G4VPhysicalVolume*>
BuildANNIEInnerStructureAsPlacements(G4LogicalVolume* mother,
                                     G4RotationMatrix* structRot,
                                     const G4ThreeVector& structCentre,
                                     G4Material* steel,
                                     G4OpticalSurface* wrapper);



#endif
