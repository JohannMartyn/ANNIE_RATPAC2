#include <GeoAnnieWbLSVolumeFactory.hh>
#include <RAT/GeoTubeFactory.hh>

#include <G4VSolid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Tubs.hh>
#include <G4Ellipsoid.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4PVPlacement.hh>



#include <G4VSolid.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Polyhedra.hh>
#include <G4Material.hh>
#include <G4Ellipsoid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <vector>

using namespace std;

namespace annie {

G4VPhysicalVolume *GeoAnnieWbLSVolumeFactory::Construct(DBLinkPtr table) {
    string volumeName = table->GetIndex();
    
    // Get WbLSVolume information
    DBLinkPtr WbLSVolumeinfo = DB::Get()->GetLink("GEO","WbLSVolume");
    vector<G4double> wblsvolume_center = WbLSVolumeinfo->GetDArray("wblsvolume_center"); // WbLSVolume centre
    G4int enable_wblsvolume = WbLSVolumeinfo->GetI("enable_wblsvolume"); // WbLSVolume centre
    G4double wblsvolume_center_x = wblsvolume_center[0];
    G4double wblsvolume_center_y = wblsvolume_center[1];
    G4double wblsvolume_center_z = wblsvolume_center[2];
    
    
    //------------ WbLSVolume position information ------------//
    // Relative to the WbLSVolume center
    G4double zpos_topcap = wblsvolume_center_z + 436.35;
    G4double zpos_botcap = wblsvolume_center_z - 436.35;
    
    G4double zpos_metalplate_top = wblsvolume_center_z + 452.23;
    G4double zpos_metalplate_bot = wblsvolume_center_z - 452.23;
    
    G4double ypos_metalplate_1 = wblsvolume_center_y;
    G4double ypos_metalplate_2 = wblsvolume_center_y;
    G4double ypos_metalplate_3 = wblsvolume_center_y - 304.05;
    G4double ypos_metalplate_4 = wblsvolume_center_y + 304.05;

    G4double xpos_metalplate_1 = wblsvolume_center_x - 304.05;
    G4double xpos_metalplate_2 = wblsvolume_center_x + 304.05;
    G4double xpos_metalplate_3 = wblsvolume_center_x;
    G4double xpos_metalplate_4 = wblsvolume_center_x;
    
    G4double rotation_radius_bridalbar = 450.0;              
    
    // The part where the info in the .geo file is overriden
    info << "Override default WbLSVolume information...\n";
    DB *db = DB::Get();
    vector<double> volume_position(3);
    // WbLSVolume vessel
    volume_position[0] = wblsvolume_center_x; volume_position[1] = wblsvolume_center_y; volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wbls_vessel","position",volume_position);
    db->SetI("GEO","wbls_vessel","enable",enable_wblsvolume);
    volume_position[2] = zpos_topcap;
    db->SetDArray("GEO","wbls_vessel_topcap","position",volume_position);
    db->SetI("GEO","wbls_vessel_topcap","enable",enable_wblsvolume);
    volume_position[2] = zpos_botcap;
    db->SetDArray("GEO","wbls_vessel_botcap","position",volume_position);
    db->SetI("GEO","wbls_vessel_botcap","enable",enable_wblsvolume);
    volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wblsvolume_liquid","position",volume_position);
    db->SetI("GEO","wblsvolume_liquid","enable",enable_wblsvolume);
    
    // Basket components
    volume_position[0] = xpos_metalplate_1; volume_position[1] = ypos_metalplate_1; volume_position[2] = zpos_metalplate_bot;
    db->SetDArray("GEO","wbls_basket_metalplate_bot1","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_bot1","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_2; volume_position[1] = ypos_metalplate_2; volume_position[2] = zpos_metalplate_bot;
    db->SetDArray("GEO","wbls_basket_metalplate_bot2","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_bot2","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_3; volume_position[1] = ypos_metalplate_3; volume_position[2] = zpos_metalplate_bot;
    db->SetDArray("GEO","wbls_basket_metalplate_bot3","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_bot3","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_4; volume_position[1] = ypos_metalplate_4; volume_position[2] = zpos_metalplate_bot;    
    db->SetDArray("GEO","wbls_basket_metalplate_bot4","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_bot4","enable",enable_wblsvolume);
    
    volume_position[0] = xpos_metalplate_1; volume_position[1] = ypos_metalplate_1; volume_position[2] = zpos_metalplate_top;
    db->SetDArray("GEO","wbls_basket_metalplate_top1","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_top1","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_2; volume_position[1] = ypos_metalplate_2; volume_position[2] = zpos_metalplate_top;
    db->SetDArray("GEO","wbls_basket_metalplate_top2","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_top2","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_3; volume_position[1] = ypos_metalplate_3; volume_position[2] = zpos_metalplate_top;
    db->SetDArray("GEO","wbls_basket_metalplate_top3","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_top3","enable",enable_wblsvolume);
    volume_position[0] = xpos_metalplate_4; volume_position[1] = ypos_metalplate_4; volume_position[2] = zpos_metalplate_top;
    db->SetDArray("GEO","wbls_basket_metalplate_top4","position",volume_position);
    db->SetI("GEO","wbls_basket_metalplate_top4","enable",enable_wblsvolume);


    
    volume_position[0] = wblsvolume_center_x+rotation_radius_bridalbar*cos(CLHEP::pi/4.); volume_position[1] = wblsvolume_center_y+rotation_radius_bridalbar*sin(CLHEP::pi/4.); volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wbls_bridal_bar1","position",volume_position);
    db->SetI("GEO","wbls_bridal_bar1","enable",enable_wblsvolume);
    volume_position[0] = wblsvolume_center_x-rotation_radius_bridalbar*cos(CLHEP::pi/4.); volume_position[1] = wblsvolume_center_y+rotation_radius_bridalbar*sin(CLHEP::pi/4.); volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wbls_bridal_bar2","position",volume_position);
    db->SetI("GEO","wbls_bridal_bar2","enable",enable_wblsvolume);
    volume_position[0] = wblsvolume_center_x+rotation_radius_bridalbar*cos(CLHEP::pi/4.); volume_position[1] = wblsvolume_center_y-rotation_radius_bridalbar*sin(CLHEP::pi/4.); volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wbls_bridal_bar3","position",volume_position);
    db->SetI("GEO","wbls_bridal_bar3","enable",enable_wblsvolume);
    volume_position[0] = wblsvolume_center_x-rotation_radius_bridalbar*cos(CLHEP::pi/4.); volume_position[1] = wblsvolume_center_y-rotation_radius_bridalbar*sin(CLHEP::pi/4.); volume_position[2] = wblsvolume_center_z;
    db->SetDArray("GEO","wbls_bridal_bar4","position",volume_position);
    db->SetI("GEO","wbls_bridal_bar4","enable",enable_wblsvolume);
    
    return NULL; //Unsure about returning NULL here but it seems not to break anything.
}

G4OpticalSurface *GeoAnnieWbLSVolumeFactory::GetSurface(string surface_name) {
    if (Materials::optical_surface.count(surface_name) == 0)
        Log::Die("error: surface "+ surface_name + " does not exist");
    return Materials::optical_surface[surface_name];
}

G4VisAttributes *GeoAnnieWbLSVolumeFactory::GetVisAttributes(DBLinkPtr table) {
    try {
        int invisible = table->GetI("invisible");
        if (invisible) return new G4VisAttributes(G4VisAttributes::Invisible);
    } catch (DBNotFoundError &e) { };
    
    G4VisAttributes *vis = new G4VisAttributes();
    
    try {
        const vector<double> &color = table->GetDArray("color");
        if (color.size() == 3) // RGB
            vis->SetColour(G4Colour(color[0], color[1], color[2]));
        else if (color.size() == 4) // RGBA
            vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
        else
            warn << "error: " << table->GetName()  << "[" << table->GetIndex() << "].color must have 3 or 4 components" << newline;
    } catch (DBNotFoundError &e) { };
    
    try {
        string drawstyle = table->GetS("drawstyle");
        if (drawstyle == "wireframe")
            vis->SetForceWireframe(true);
        else if (drawstyle == "solid")
            vis->SetForceSolid(true);
        else
            warn << "error: " << table->GetName() << "[" << table->GetIndex() << "].drawstyle must be either \"wireframe\" or \"solid\".";
    } catch (DBNotFoundError &e) { };
    
    try {
        int force_auxedge = table->GetI("force_auxedge");
        vis->SetForceAuxEdgeVisible(force_auxedge == 1);
    } catch (DBNotFoundError &e) { };

    return vis;
}

} // namespace RAT
