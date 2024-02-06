#include "GeoANNIEFactory.hh"
#include <RAT/Materials.hh>
#include <RAT/DB.hh>
#include <RAT/Log.hh>

#include <G4VSolid.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include <G4UnionSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Polyhedra.hh>
#include <G4Material.hh>
#include <G4Ellipsoid.hh>
#include <G4VPhysicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4GDMLParser.hh>
#include <G4Color.hh>
#include <vector>

#include <fstream>

using namespace std;

namespace RAT {

    G4VPhysicalVolume *GeoANNIEFactory::Construct(DBLinkPtr table) {
        string volumeName = table->GetIndex();
        
        // Get references to the mother volume
        G4LogicalVolume *motherLog = FindMother(table->GetS("mother"));
        //G4VPhysicalVolume *motherPhys = FindPhysMother(table->GetS("mother"));
        
        // Get InnerStructure information
        DBLinkPtr dbinfo = DB::Get()->GetLink("GEO","InnerStructure_Holders_Blacksheets");

        vector<G4double> inner_structure_center = dbinfo->GetDArray("inner_structure_center"); // InnerStructure center
        G4int enable_structure = dbinfo->GetI("enable_inner_structure");
        G4String gdml_file = dbinfo->GetS("inner_structure_gdml_file");
        G4double rot_angle = dbinfo->GetD("inner_structure_rotation_angle");
        G4String wrapper_material = dbinfo->GetS("inner_structure_wrapper_material"); //tyvek?

        G4int enable_annieholders = dbinfo->GetI("enable_annie_holders");
        G4int enable_luxetelholders = dbinfo->GetI("enable_luxetel_holders");
        G4int enable_blacksheets = dbinfo->GetI("enable_black_sheets");
        
        G4int write_gdml = dbinfo->GetI("write_gdml"); // InnerStructure center

        G4GDMLParser parser;
        
        if(enable_annieholders != 0){
            std::vector<double> color(4);
            color[0] = 0.2; color[1] = 0.2; color[3] = 0.2; color[4] = 0.2;
            int invisible = 1;
            try {
                color = dbinfo->GetDArray("annie_holders_color");
                if (color.size() != 3 && color.size() != 4){  // RGB
                    warn << "GeoANNIEFactory error: " << dbinfo->GetName() << "[" << dbinfo->GetIndex() << "].color must have 3 or 4 components" << newline;
                }
            }
            catch (DBNotFoundError &e) {};
            try {
                invisible = dbinfo->GetI("annie_holders_invisible");
            }
            catch (DBNotFoundError &e) {};
            ConstructANNIEHolders(motherLog, color, invisible);
        }
        if(enable_luxetelholders != 0){
            std::vector<double> color(4);
            int invisible = 1;
            color[0] = 0.2; color[1] = 0.2; color[3] = 0.2; color[4] = 0.2;
            try {
                color = dbinfo->GetDArray("luxetel_holders_color");
                if (color.size() != 3 && color.size() != 4){  // RGB
                    warn << "GeoANNIEFactory error: " << dbinfo->GetName() << "[" << dbinfo->GetIndex() << "].color must have 3 or 4 components" << newline;
                }
            }
            catch (DBNotFoundError &e) {};
            try {
                invisible = dbinfo->GetI("luxetel_holders_invisible");
            }
            catch (DBNotFoundError &e) {};
            ConstructLUXETELHolders(motherLog, color, invisible);
        }
        if(enable_blacksheets != 0){
            std::vector<double> color(4);
            color[0] = 0.2; color[1] = 0.2; color[3] = 0.2; color[4] = 0.2;
            int invisible = 1;
            try {
                color = dbinfo->GetDArray("black_sheet_color");
                if (color.size() != 3 && color.size() != 4){  // RGB
                    warn << "GeoANNIEFactory error: " << dbinfo->GetName() << "[" << dbinfo->GetIndex() << "].color must have 3 or 4 components" << newline;
                }
            }
            catch (DBNotFoundError &e) {};
            try {
                invisible = dbinfo->GetI("black_sheet_invisible");
            }
            catch (DBNotFoundError &e) {};
            ConstructBlackSheet(motherLog, color, invisible);
        }

        if(write_gdml != 0){
            G4String gdml_out_file = dbinfo->GetS("output_gdml_file");
            G4cout<<"Writing GDML output file"<<G4endl;
            parser.Write(gdml_out_file, motherLog);
            G4cout<<"GDML file "<<gdml_out_file<<" written"<<G4endl;
        }

        if(enable_structure != 0){
            // Get structure from GDML file
            G4VPhysicalVolume* innerstructure_phys;
            G4LogicalVolume* innerstructure_log;
            G4VPhysicalVolume* innerstructure_phys_placement;
            
            G4ThreeVector StructureCenter(inner_structure_center[0],inner_structure_center[1],inner_structure_center[2]);  
            G4RotationMatrix* rotm = new G4RotationMatrix();
            rotm->rotateZ(rot_angle*CLHEP::deg);

            parser.Read(gdml_file);
            innerstructure_phys = parser.GetWorldVolume();

            innerstructure_log = innerstructure_phys->GetLogicalVolume();
            innerstructure_phys_placement = new G4PVPlacement(rotm, StructureCenter, innerstructure_log, "innerstructure_phys", motherLog, false, 0, false);


            G4LogicalSkinSurface* InnerStructureSurface_log = new G4LogicalSkinSurface( "innerStructureSurface", innerstructure_log, Materials::optical_surface[wrapper_material]);

            // Optional visualization parts
            G4VisAttributes *vis = new G4VisAttributes();
            try {
                const std::vector<double> &color = dbinfo->GetDArray("inner_structure_color");
                if (color.size() == 3){  // RGB
                    vis->SetColour(G4Colour(color[0], color[1], color[2]));
                }
                else if (color.size() == 4){ // RGBA
                    vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
                }
                else{
                    warn << "GeoANNIEFactory error: " << dbinfo->GetName() << "[" << dbinfo->GetIndex() << "].color must have 3 or 4 components" << newline;
                }
            }
            catch (DBNotFoundError &e) {};

            //try {
            //    std::string drawstyle = dbinfo->GetS("drawstyle");
            //    if (drawstyle == "wireframe"){
            //        vis->SetForceWireframe(true);
            //    }
            //    else if (drawstyle == "solid"){
            //        vis->SetForceSolid(true);
            //    }
            //    else{
            //        warn << "GeoSolidFactory error: " << dbinfo->GetName() << "[" << dbinfo->GetIndex() << "].drawstyle must be either \"wireframe\" or \"solid\".";
            //    }
            //}
            //catch (DBNotFoundError &e) {};

            // Check for invisible flag last
            try {
                int invisible = dbinfo->GetI("inner_structure_invisible");
                if (invisible != 0) innerstructure_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            }
            catch (DBNotFoundError &e) {};

            innerstructure_log->SetVisAttributes(vis);

            return innerstructure_phys_placement;
        }
        else {
            return NULL; 
        }
    }
    
    void GeoANNIEFactory::ConstructANNIEHolders(G4LogicalVolume *motherLog, const std::vector<double> &color, const G4int invisible){

        //Code copied fromWCSim and adopted to RATPAC

        G4int WCBarrelRingNPhi = 8;
        G4double dPhi = 45.0;

        G4cout <<"Construct ANNIE Holders: " <<G4endl;

        //Tried to get rough dimensions of the ANNIE holder from the laser scan file
        //Not really sure about the thickness, assume 2cm thickness for now (should not be super important)
        //G4Box *ANNIEHolder_Box = new G4Box("ANNIEHolder_Box",10.5*CLHEP::cm,17.75*CLHEP::cm,1.*CLHEP::cm);
        G4Box *ANNIEHolder_Box = new G4Box("ANNIEHolder_Box",10.5*CLHEP::cm,16.75*CLHEP::cm,1.*CLHEP::cm); // Make holder slightly less wide to prevent geometry overlaps
        G4Tubs *ANNIEHolder_Tube = new G4Tubs("ANNIEHolder_Tube",0.0*CLHEP::cm,6.0*CLHEP::cm,1.1*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);

        //Create combined logical volume of the Box + Tube to get holder with hole (Subtraction Solid)

        G4SubtractionSolid *solidANNIEHolder = new G4SubtractionSolid("ANNIEHolder",ANNIEHolder_Box,ANNIEHolder_Tube,0,G4ThreeVector(0.,0.,0.));

        //Check the material of the ANNIE holders somewhere!
        //ANNIE holders should be made out of polyethylene
        //Assume white acrylic since it should be similar
        G4LogicalVolume *logANNIEHolder = new G4LogicalVolume(solidANNIEHolder,G4Material::GetMaterial("acrylic_white"),"WCANNIEHolder",0,0,0);
        G4LogicalSkinSurface* ANNIEHolderSurface_log = new G4LogicalSkinSurface("ANNIEHolderSurface", logANNIEHolder, Materials::optical_surface["acrylic_white"]);

        G4VisAttributes *vis = new G4VisAttributes();
        if (color.size() == 3){  // RGB
            vis->SetColour(G4Colour(color[0], color[1], color[2]));
        }
        else if (color.size() == 4){ // RGBA
            vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
        }
        if (invisible != 0) logANNIEHolder->SetVisAttributes(G4VisAttributes::GetInvisible());
        logANNIEHolder->SetVisAttributes(vis);

        //G4double dist_pmt_holder = 10.84;     //Holder is 20cm away from the front face of the ANNIE PMTs, WCSim center is 9.16cm away from front --> 10.84cm distance
        G4double dist_pmt_holder = 7.84;        //Reduce distance by 3cm to prevent geometry overlaps

        //Read in PMT positions again, and project position of holder positions from the PMT positions

        // Create rotation matrices for the orientations of the PMTs
        std::vector<G4RotationMatrix*> holder_rotation_matrices;
        // Bottom PMTs have panel number 0
        G4RotationMatrix *WCBottomCapRotation = new G4RotationMatrix();
        holder_rotation_matrices.push_back(WCBottomCapRotation);
        G4RotationMatrix* WCPMTRotation = new G4RotationMatrix;
        WCPMTRotation->rotateY(90.*CLHEP::deg);    
        // Barrel PMTs have panel numbers 1-8
        for(G4int facei=0; facei<WCBarrelRingNPhi; facei++){
            G4RotationMatrix* WCPMTRotationNext = new G4RotationMatrix(*WCPMTRotation);
            WCPMTRotationNext->rotateX((dPhi*CLHEP::deg*facei)-67.5*CLHEP::deg+180.0*CLHEP::deg);
            //WCPMTRotationNext->rotateX((dPhi*CLHEP::deg*facei));
            holder_rotation_matrices.push_back(WCPMTRotationNext);
        }
        // Top PMTs have panel number 9
        G4RotationMatrix *WCTopCapRotation = new G4RotationMatrix();
        WCTopCapRotation->rotateY(180.*CLHEP::deg);
        holder_rotation_matrices.push_back(WCTopCapRotation);

        //Select only ANNIE PMTs and propagate their position outwards to get central holder position
        std::ifstream pmt_position_file("PMTPositions_Scan.txt");
        std::string next_pmt;
        G4double pmt_x, pmt_y, pmt_z, pmt_dirx, pmt_diry, pmt_dirz;
        G4double holder_x, holder_y, holder_z;
        G4int panel_nr, pmt_type;
        G4int HolderID;
        while (!pmt_position_file.eof()){
            pmt_position_file >> HolderID >> panel_nr >> pmt_x >> pmt_y >> pmt_z >> pmt_dirx >> pmt_diry >> pmt_dirz >> pmt_type;
            if (pmt_position_file.eof()) break;            
            if (pmt_type == 2){ //select only ANNIE (Hamamatsu 8inch) PMTs for the holders on the side
                G4RotationMatrix *holder_rot = holder_rotation_matrices.at(panel_nr);

                G4double phi_temp = (dPhi*CLHEP::deg*(panel_nr-1.0))-67.5*CLHEP::deg+180.0*CLHEP::deg;

                //Shift the PMT position outwards
                pmt_x += (cos(phi_temp)*dist_pmt_holder);
                pmt_z += (sin(phi_temp)*dist_pmt_holder);

                holder_x = pmt_x*CLHEP::cm;
                holder_y = (168.1-pmt_z)*CLHEP::cm;
                holder_z = ((pmt_y+14.45))*CLHEP::cm;

                G4cout <<HolderID <<"\t" << panel_nr <<"\t" << holder_x<<","<<holder_y<<","<<holder_z<<G4endl;
                
                G4ThreeVector HolderPosition(holder_x,holder_y,holder_z);
                G4VPhysicalVolume *physicalHolder = new G4PVPlacement(holder_rot,   //its rotation
                                                                    HolderPosition,     //its position
                                                                    logANNIEHolder,         //its logical volume
                                                                    "ANNIEHolder",         //its name
                                                                    motherLog,      //its mother volume
                                                                    false,              //no boolean operations
                                                                    HolderID,               //ID for this PMT (=channelkey in data)
                                                                    true);              //check overlaps
            }
        }
        pmt_position_file.close();

    }

    void GeoANNIEFactory::ConstructLUXETELHolders(G4LogicalVolume *motherLog, const std::vector<double> &color, const G4int invisible){

        //Code copied fromWCSim and adopted to RATPAC

        G4cout <<"Construct LUX/ETEL Holders"<<G4endl;

        //Dimensions for LUX holders taken from this presentation:
        //https://annie-docdb.fnal.gov/cgi-bin/sso/RetrieveFile?docid=654&filename=LUXPMTs_Phone_2017-04-13_FISCHER.pdf&version=1
        //Thickness: 3/4" (1.905CLHEP::cm, half-height: ~0.95CLHEP::cm), Width: 6" (15.24CLHEP::cm), Length: 2x7"+6" = 20" (50.8CLHEP::cm)
        //Hole has a diameter of 6.625" (r=8.41375CLHEP::cm)
        //

        //Dimensions for ETEL holders are assumed to be the same, no reference though   

        //G4Box *LUXHolder_Box = new G4Box("ANNIEHolder_Box",7.62*CLHEP::cm,25.4*CLHEP::cm,0.95*CLHEP::cm);
        G4Box *LUXHolder_Box = new G4Box("ANNIEHolder_Box",7.62*CLHEP::cm,21.4*CLHEP::cm,0.95*CLHEP::cm); //Make holders smaller to prevent geometry overlaps
        //G4Tubs *LUXHolder_Tube = new G4Tubs("LUXHolder_Tube",0.0*CLHEP::cm,8.41375*CLHEP::cm,0.95*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);
        //G4Tubs *LUXHolder_Tube = new G4Tubs("LUXHolder_Tube",0.0*CLHEP::cm,8.41375*CLHEP::cm,2.74*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);
        //Distance between holder and PMT: 5.5CLHEP::cm (-> 2.75CLHEP::cm) + 18 CLHEP::cm housing height (->9CLHEP::cm): 11.75CLHEP::cm
        G4Tubs *LUXHolder_Tube = new G4Tubs("LUXHolder_Tube",0.0*CLHEP::cm,8.41375*CLHEP::cm,11.74*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);

        //Create combined logical volume of the Box + Tube to get holder with hole (Subtraction Solid)

        G4UnionSolid *solidLUXHolder = new G4UnionSolid("LUXHolder",
                                                LUXHolder_Box,
                                                LUXHolder_Tube,
                                                0,
                                                G4ThreeVector(0.,0.,-6.25*CLHEP::cm));


        //LUX & ETEL holders are made of Schedule 80 pvc_black --> Use pvc_black as material
        G4LogicalVolume *logLUXHolder = new G4LogicalVolume(solidLUXHolder,
                G4Material::GetMaterial("pvc_black"),
                "WCLUXHolder",
                0,0,0);
        G4LogicalSkinSurface* LUXHolderSurface_log = new G4LogicalSkinSurface( "LUXHolderSurface", logLUXHolder, Materials::optical_surface["pvc_black"]);

        //G4double dist_pmt_holder_lux = 6.0;       //LUX center is 11.7CLHEP::cm from glass front surface total distance glass surface-wings = 17.7CLHEP::cm, dist = 6.0CLHEP::cm
        G4double dist_pmt_holder_lux = 5.5;     //Slightly reduce distance from 6 to 5.5 CLHEP::cm to prevent geometry overlaps

        //G4Tubs *ETELHolder_Tube = new G4Tubs("ETELHolder_Tube",0.0*CLHEP::cm,8.41375*CLHEP::cm,5.62*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);
        //Distance ETEL holder /PMT: 11.25CLHEP::cm (->5.625CLHEP::cm) + 18CLHEP::cm housing height (->9CLHEP::cm): 14.625CLHEP::cm
        G4Tubs *ETELHolder_Tube = new G4Tubs("ETELHolder_Tube",0.0*CLHEP::cm,8.41375*CLHEP::cm,14.62*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);
        G4UnionSolid *solidETELHolder = new G4UnionSolid("ETELHolder",
                                            LUXHolder_Box,
                                            ETELHolder_Tube,
                                            0,
                                            G4ThreeVector(0.,0.,3.375*CLHEP::cm));

        G4LogicalVolume *logETELHolder = new G4LogicalVolume(solidETELHolder,
                G4Material::GetMaterial("pvc_black"),
                "WCETELHolder",
                0,0,0);
        G4LogicalSkinSurface* ETELHolderSurface_log = new G4LogicalSkinSurface( "ETELHolderSurface", logETELHolder, Materials::optical_surface["pvc_black"]);
        

        //G4double dist_pmt_holder_etel = 7.25; //ETEL center is 11.8CLHEP::cm from glass front surface, total distance from glass surface to wings is 19.05CLHEP::cm (7.5") -> dist = 19.05CLHEP::cm-11.8CLHEP::cm = 7.25CLHEP::cm
        G4double dist_pmt_holder_etel = 11.25;  //Try to get ETEL wings above the top part of the Inner Structure--> increase distance


        //Create Rotation matrix for PMT holders
        G4RotationMatrix* WCPMTRotation = new G4RotationMatrix;

        G4VisAttributes *vis = new G4VisAttributes();
        if (color.size() == 3){  // RGB
            vis->SetColour(G4Colour(color[0], color[1], color[2]));
        }
        else if (color.size() == 4){ // RGBA
            vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
        }
        if (invisible != 0){
            logLUXHolder->SetVisAttributes(G4VisAttributes::GetInvisible());
            logETELHolder->SetVisAttributes(G4VisAttributes::GetInvisible());
        }
        logLUXHolder->SetVisAttributes(vis);
        logETELHolder->SetVisAttributes(vis);

        //Select only ETEL + LUX PMTs and propagate their position up-/downwards to get central holder position
        std::ifstream pmt_position_file("PMTPositions_Scan.txt");
        std::string next_pmt;
        double pmt_x, pmt_y, pmt_z, pmt_dirx, pmt_diry, pmt_dirz;
        double holder_x, holder_y, holder_z;
        int panel_nr, pmt_type;
        int HolderID;
        while (!pmt_position_file.eof()){
            pmt_position_file >> HolderID >> panel_nr >> pmt_x >> pmt_y >> pmt_z >> pmt_dirx >> pmt_diry >> pmt_dirz >> pmt_type;
            if (pmt_position_file.eof()) break;
            //G4cout << "Read in PMT "<<HolderID<<", panel nr: "<<panel_nr<<", Position ("<<pmt_x<<","<<pmt_y<<","<<pmt_z<<"), PMT type: "<<pmt_type<<G4endl;
            
            if (fabs(pmt_diry-1.) < 0.00001) {  //select only LUX PMTs for the holders (pointing upwards)
                //G4RotationMatrix *holder_rot = holder_rotation_matrices.at(panel_nr);
                //Shift the PMT position outwards

                G4RotationMatrix *holder_rot = new G4RotationMatrix(*WCPMTRotation);
                holder_rot->rotateZ((45+90)*CLHEP::deg);
                pmt_x -= (pmt_dirx*dist_pmt_holder_lux);
                pmt_y -= (pmt_diry*dist_pmt_holder_lux);
                pmt_z -= (pmt_dirz*dist_pmt_holder_lux);

                holder_x = pmt_x*CLHEP::cm;
                holder_y = (168.1-pmt_z)*CLHEP::cm;
                holder_z = ((pmt_y+14.45))*CLHEP::cm;

                //G4cout <<"Edited LUX Holder position ("<<holder_x<<","<<holder_y<<","<<holder_z<<")"<<G4endl;

                G4ThreeVector HolderPosition(holder_x,holder_y,holder_z);
                G4VPhysicalVolume *physicalHolder = new G4PVPlacement(holder_rot,   //its rotation
                                    HolderPosition,         //its position
                                    logLUXHolder,           //its logical volume
                                    "LUXHolder",           //its name
                                    motherLog,          //its mother volume
                                    false,              //no boolean operations
                                    HolderID,           //ID for this PMT (=channelkey in data)
                                    true);              //check overlaps
            }
            else if (fabs(pmt_diry+1.) < 0.00001) {       //select only ETEL PMTs for the holders (pointing downwards)
                
                pmt_x -= (pmt_dirx*dist_pmt_holder_etel);
                pmt_y -= (pmt_diry*dist_pmt_holder_etel);
                pmt_z -= (pmt_dirz*dist_pmt_holder_etel);

                holder_x = pmt_x*CLHEP::cm;
                holder_y = (168.1-pmt_z)*CLHEP::cm;
                holder_z = ((pmt_y+14.45))*CLHEP::cm;

                //G4cout <<"Edited ETEL Holder position ("<<holder_x<<","<<holder_y<<","<<holder_z<<")"<<G4endl;

                G4RotationMatrix *holder_rot = new G4RotationMatrix(*WCPMTRotation);
                double phi = atan2(holder_x,holder_y);
                phi = (phi > 0)? phi : 2*CLHEP::pi+phi;
                //There are 8 different rotations for the top PMT holders, depending on the phi positions of the PMTs
                for (int i_phi = 0; i_phi < 8; i_phi++){
                    double lower_phi = i_phi*CLHEP::pi/4.-CLHEP::pi/8.;
                    double upper_phi = i_phi*CLHEP::pi/4.+CLHEP::pi/8.;
                    if (lower_phi <= phi && phi <= upper_phi) holder_rot->rotateZ((i_phi*45)*CLHEP::deg);
                    else {
                        lower_phi += 2*CLHEP::pi;
                        upper_phi += 2*CLHEP::pi;
                        if (lower_phi <= phi && phi <= upper_phi) holder_rot->rotateZ((i_phi*45)*CLHEP::deg);
                    }
                }
                //4 holder in the inner ring are rotated by 90CLHEP::degrees w.r.t. the holders in the outer ring
                if (sqrt(holder_x*holder_x+holder_y*holder_y) < 500.0) holder_rot->rotateZ(90*CLHEP::deg);
                G4ThreeVector HolderPosition(holder_x,holder_y,holder_z);
                G4VPhysicalVolume *physicalHolder = new G4PVPlacement(holder_rot,   //its rotation
                                    HolderPosition,         //its position
                                    logETELHolder,          //its logical volume
                                    "ETELHolder",          //its name
                                    motherLog,          //its mother volume
                                    false,              //no boolean operations
                                    HolderID,           //ID for this PMT (=channelkey in data)
                                    true);              //check overlaps
            }
            
        }
        pmt_position_file.close();
    }

    void GeoANNIEFactory::ConstructBlackSheet(G4LogicalVolume *motherLog, const std::vector<double> &color, const G4int invisible){

        //Code copied fromWCSim and adopted to RATPAC

        G4cout <<"Construct Black Sheets"<<G4endl;

        // PMT rings and faces counts
        G4int WCBarrelNumPMTHorizontal = 16;      // this + WCPMTperCellHorizontal define num detector sides
        G4int WCPMTperCellHorizontal   = 2;       // ^ for octagonal inner structure it must produce 8
        G4int WCBarrelRingNPhi         = 8;       // number of faces in the inner structure
        G4int numhatchpmts = 4;

        // Offsets and positioning
        G4double WCIDHeight               = 3.96*CLHEP::m;    // full height
        G4double WCIDDiameter             = 2.554*CLHEP::m;   // 2x shortest distance to the centre of a ocatgonal
                          // cell wall- from blueprints, this is 100.57" = 255.4cm
                          // from blueprints of inner structure diameter is 106.64",
                          // hexagonal side is 40.81", 100.57" from face-to-face
                          // (note: OUTER dimensions, including steel bar width)
        G4double WCIDRadius = WCIDDiameter/2.;

        G4double WCBarrelPMTOffset        = 0.415*CLHEP::m;     // offset of first barrel ring from tank caps  0.4 -> .34?
        G4double WCBlackSheetThickness    = 1.01*CLHEP::mm;   // liner is 40 mil. = 40 milli inches. 
        G4double InnerStructureCentreOffset = -5.6*CLHEP::cm;   // centre of inner structure is closer to the ground


        G4double WCBarrelCellStartPhi = 0.0*CLHEP::deg;
        G4double totalAngle = 360.0*CLHEP::deg;

        //-----------------------------------------------------------
        // The Blacksheet, a daughter of the cells containing PMTs,
        // and also some other volumes to make the edges light tight
        //-----------------------------------------------------------
        
        G4double BlackSheetRadius = WCIDRadius + 3*CLHEP::cm;

        G4double annulusBlackSheetRmax[2] = {(BlackSheetRadius+WCBlackSheetThickness),
                                              BlackSheetRadius+WCBlackSheetThickness};
        G4double annulusBlackSheetRmin[2] = {(BlackSheetRadius),
                                              BlackSheetRadius};
        G4double mainAnnulusHeight = WCIDHeight -2.*WCBarrelPMTOffset;
        mainAnnulusHeight += 30*CLHEP::cm; //WCBlackSheet needs to extend to above ETEL holders and below LUX housings to match reality
        G4double mainAnnulusZ[2] = {-mainAnnulusHeight/2., mainAnnulusHeight/2};
        
        G4Polyhedra* solidWCBarrelCellBlackSheet = new G4Polyhedra("WCBarrelCellBlackSheet",
                                                                    WCBarrelCellStartPhi, //phi start
                                                                    totalAngle,           //total phi
                                                                    WCBarrelRingNPhi,     //NPhi-gon
                                                                    2,
                                                                    mainAnnulusZ,
                                                                    annulusBlackSheetRmin,
                                                                    annulusBlackSheetRmax);
        
        G4LogicalVolume* logicWCBarrelCellBlackSheet =
            new G4LogicalVolume(solidWCBarrelCellBlackSheet,
                                G4Material::GetMaterial("polyethylene_black"),
                                "WCBarrelCellBlackSheet",
                                0,0,0);
        G4LogicalSkinSurface* WCBarrelCellBlackSheetSurface_log = new G4LogicalSkinSurface( "WCBarrelCellBlackSheetSurface_log", logicWCBarrelCellBlackSheet, Materials::optical_surface["polyethylene_black"]);

        G4VisAttributes *vis = new G4VisAttributes();
        if (color.size() == 3){  // RGB
            vis->SetColour(G4Colour(color[0], color[1], color[2]));
        }
        else if (color.size() == 4){ // RGBA
            vis->SetColour(G4Colour(color[0], color[1], color[2], color[3]));
        }
        if (invisible != 0){
            logicWCBarrelCellBlackSheet->SetVisAttributes(G4VisAttributes::GetInvisible());
        }
        logicWCBarrelCellBlackSheet->SetVisAttributes(vis);

        G4VPhysicalVolume* physiWCBarrelCellBlackSheet =
            new G4PVPlacement(0,
                              G4ThreeVector(0.,0.,InnerStructureCentreOffset-5.0*CLHEP::cm),
                              logicWCBarrelCellBlackSheet,
                              "WCBarrelCellBlackSheet",
                              motherLog, //logicWCBarrel,
                              false,
                              0,true);
        G4cout<<"Constructed barrel cell blacksheet with radius "<<WCIDRadius<<" to "<<(WCIDRadius+WCBlackSheetThickness)<<G4endl;

        for(G4int zflip = -1; zflip<=1; zflip+=2){
            //--------------------------------------------------------------------
            // ---------------------Add cap blacksheet----------------------------
            // -------------------------------------------------------------------
            
            G4double capBlackSheetZ[2] = {-WCBlackSheetThickness*zflip, 0.};
            G4double capBlackSheetRmin[2] = {0., 0.};
            G4double capBlackSheetRmax[2] = {BlackSheetRadius+WCBlackSheetThickness, 
                                             BlackSheetRadius+WCBlackSheetThickness};
            G4VSolid* solidWCCapBlackSheet;
            if(WCBarrelRingNPhi*WCPMTperCellHorizontal == WCBarrelNumPMTHorizontal){
                solidWCCapBlackSheet
                    = new G4Polyhedra("WCCapBlackSheet",  // name
                                      0.*CLHEP::deg,             // phi start
                                      totalAngle,         // total phi
                                      WCBarrelRingNPhi,   // NPhi-gon
                                      2,                  // z-planes
                                      capBlackSheetZ,     // position of the Z planes
                                      capBlackSheetRmin,  // min radius at the z planes
                                      capBlackSheetRmax   // max radius at the Z planes
                                      );
                // G4cout << *solidWCCapBlackSheet << G4endl;
            }
            
            G4LogicalVolume* logicWCCapBlackSheet;
            G4SubtractionSolid* solidWCCapBlackSheetHole = (G4SubtractionSolid*) solidWCCapBlackSheet;
            if (zflip == 1){
                //Select only ETEL + LUX PMTs and propagate their position up-/downwards to get central holder position
                std::ifstream pmt_position_file("PMTPositions_Scan.txt");
                std::string next_pmt;
                double pmt_x, pmt_y, pmt_z, pmt_dirx, pmt_diry, pmt_dirz;
                double hole_x, hole_y, hole_z;
                int panel_nr, pmt_type;
                int HolderID;
                while (!pmt_position_file.eof()){
                    pmt_position_file >> HolderID >> panel_nr >> pmt_x >> pmt_y >> pmt_z >> pmt_dirx >> pmt_diry >> pmt_dirz >> pmt_type;
                    if (pmt_position_file.eof()) break;
                    if (fabs(pmt_diry+1.) < 0.00001) {       //select only ETEL PMTs for the holders (pointing downwards)

                    hole_x = pmt_x*CLHEP::cm;
                    hole_y = (168.1-pmt_z)*CLHEP::cm;
                    hole_z = ((pmt_y+14.45))*CLHEP::cm;

                    G4Tubs *WCCap_Hole = new G4Tubs("WCCap_Hole",0.0*CLHEP::cm,8.414*CLHEP::cm,WCBlackSheetThickness+0.1*CLHEP::cm,0*CLHEP::deg,360*CLHEP::deg);

                    //Create combined logical volume of the Box + Tube to get holder with hole (Subtraction Solid)

                    solidWCCapBlackSheetHole = new G4SubtractionSolid("WCCapBlackSheetHole",
                                                    solidWCCapBlackSheetHole,
                                                    WCCap_Hole,
                                                    0,
                                                    G4ThreeVector(hole_x,hole_y,0.));


                    }

                }
                pmt_position_file.close();

                logicWCCapBlackSheet =
                new G4LogicalVolume(solidWCCapBlackSheetHole,
                                    G4Material::GetMaterial("polyethylene_black"),
                                    "WCCapBlackSheet",
                                    0,0,0);

            } else {

              logicWCCapBlackSheet =
                new G4LogicalVolume(solidWCCapBlackSheet,
                                    G4Material::GetMaterial("polyethylene_black"),
                                    "WCCapBlackSheet",
                                    0,0,0);

            }
            
            if (invisible != 0){
                logicWCCapBlackSheet->SetVisAttributes(G4VisAttributes::GetInvisible());
            }
            logicWCCapBlackSheet->SetVisAttributes(vis);

            G4double capAssemblyHeight = mainAnnulusHeight/2+1*CLHEP::mm+WCBlackSheetThickness;
            
            G4double AssemblyHeight = capAssemblyHeight*zflip + InnerStructureCentreOffset - 5.0*CLHEP::cm;

            G4LogicalSkinSurface* WCCapBlackSheetSurface_log = new G4LogicalSkinSurface( "WCCapBlackSheetSurface_log", logicWCCapBlackSheet, Materials::optical_surface["polyethylene_black"]);

            G4VPhysicalVolume* physiWCCapBlackSheet =
                new G4PVPlacement(0,
                                  G4ThreeVector(0.,0.,AssemblyHeight),
                                  logicWCCapBlackSheet,
                                  "WCCapBlackSheet",
                                  motherLog,
                                  false,
                                  0,
                                  true);

            G4cout<<"constructed cap blacksheet at height "<<AssemblyHeight<<G4endl;
        }
    }
} // namespace RAT

