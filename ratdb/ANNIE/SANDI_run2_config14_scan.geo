////////////////////////////////////////////////////////
// ANNIE detector geometry for Phase 2 
// 
// Config 14: 	- 20 LUX 10inch PMTs at bottom 
//	     	- 48 10inch Watchboy, 40 8inch hqe, 4 10inch HQE Watchman
//          	- 20 11 inch ETEL PMT on top 
//		- PMT positions based on laser scan file from WCSim
// Author: J. Martyn <jomartyn@uni-mainz.de>
//
////////////////////////////////////////////////////////

{
name: "GEO",
index: "world",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "", // world volume has no mother
type: "box",
size: [15000.0, 15000.0, 15000.0], // mm, half-length
material: "rock",
invisible: 1,
}

{
name: "GEO",
index: "hall",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "world", 
type: "box",
//size: [3910.0, 2845.0, 4250.0], // mm, half-length
size: [3910.0, 3055.0, 4250.0], // mm, half-length
position: [0.0, 2268.8, -1070.2],
color: [0.5, 1.0, 0.0, 0.1],
material: "air",
invisible: 1,
rotation: [90.0, 0.0, 00.0],
}

/////////////////////////////////////////////////////////////////////
///////////////********** Tank volumes ***********///////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "tank",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "hall", 
type: "tube",
r_max: 1524.0,
size_z: 1981.2,
position: [0.0, -1070.2, -2268.8],
material: "stainless_steel",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 1,
drawstyle: "solid",
rotation: [-90.0, 0.0, 00.0],
}

{
name: "GEO",
index: "liner",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "tank",
type: "tube",
r_max: 1517.6,
size_z: 1974.8,
position: [0.0, 0.0, 0.0],
material: "acrylic_black",
color: [1.0, 0.5, 0.5, 0.1],
invisible: 1,
drawstyle: "solid",
}

{
name: "GEO",
index: "detector",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "liner",
type: "tube",
r_max: 1516.6,
size_z: 1973.8,
position: [0.0, 0.0, 0.0],
material: "water_gdS_0p2",
color: [0.6, 0.8, 1.0, 0.1],
invisible: 1,
drawstyle: "solid",
}


{
name: "GEO",
index: "white_ring_top",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector", 
type: "tube",
r_max: 460.0,
r_min: 340.0, 
size_z: 10.0,
position: [0.0, 0.0, 1556.0],
material: "polyethylene_white",
surface: "polyethylene_white",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
enable: 1,
drawstyle: "solid",
}

/////////////////////////////////////////////////////////////////////
///////////********** End of tank volumes ***********////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
///////////////********** PMT arrays ***********/////////////////////
/////////////////////////////////////////////////////////////////////

{ 
name: "GEO", 
index: "bottom_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_ANNIE", // LUX 10inch
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_bottomgrid_QECORR",
orientation: "manual",
} 

{ 
name: "GEO", 
index: "top_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "d784kflb", //ETEL 11inch
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_topgrid_SANDI_QECORR",
orientation: "manual",
} 

// Side ring 1 (lower)
{ 
name: "GEO", 
index: "side_ring_1_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_ANNIE", // Watchboy 10 inch
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_1_QECORR",
orientation: "manual",
} 

// Side ring 2-1
{ 
name: "GEO", 
index: "side_ring_2_1_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r5912_hqe_ANNIE", 
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_2_1_QECORR",
orientation: "manual",
} 

// Side ring 2-2
{ 
name: "GEO", 
index: "side_ring_2_2_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_ANNIE", 
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_2_2_QECORR",
orientation: "manual",
} 

//Side ring 3 
{ 
name: "GEO", 
index: "side_ring_3_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r5912_hqe_ANNIE", 
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_3_QECORR",
orientation: "manual", 
} 

//Side ring 4-1
{ 
name: "GEO", 
index: "side_ring_4_1_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_ANNIE",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_4_1_QECORR",
orientation: "manual", 
} 

//Side ring 4-2
{ 
name: "GEO", 
index: "side_ring_4_2_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_hqe_ANNIE",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_4_2_QECORR",
orientation: "manual",  
} 

//Side ring 5
{ 
name: "GEO", 
index: "side_ring_5_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r5912_hqe_ANNIE", 
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_5_QECORR",
orientation: "manual", 
} 

//Side ring 6
{ 
name: "GEO", 
index: "side_ring_6_pmts", 
enable: 1,
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "pmtarray", 
pmt_model: "r7081_ANNIE",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
pos_table: "PMTINFO_config14_scan_side_ring_6_QECORR",
orientation: "manual", 
}


//added by MK
//LAPPDs
//{ 
//name: "GEO", 
//index: "side_ring_lappds", 
//enable: 1,
//valid_begin: [0, 0], 
//valid_end: [0, 0], 
//mother: "detector", 
//type: "pmtarray", 
//pmt_model: "lappd",
//pmt_detector_type: "idpmt",
//sensitive_detector: "/mydet/pmt/inner", 
////efficiency_correction: 1.000,  
//pos_table: "LAPPDINFO_config14_scan_side_ring",
//orientation: "manual", 
//}

/////////////////////////////////////////////////////////////////////
///////////********** End of PMT arrays ***********//////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
///////////** Start of Watchman/boy holder rings **//////////////////
/////////////////////////////////////////////////////////////////////

{
name: "GEO",
index: "holder_ring_config14_scan_side_ring_1",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tubearray",
r_max: 134.0,
r_min: 107.0,
size_z: 3.1,
pos_table: "RINGINFO_config14_scan_side_ring_1",
orientation: "manual",
material: "acrylic_uvt_good",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
drawstyle: "solid",
}
{
name: "GEO",
index: "holder_ring_config14_scan_side_ring_2_2",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tubearray",
r_max: 134.0,
r_min: 107.0,
size_z: 3.1,
pos_table: "RINGINFO_config14_scan_side_ring_2_2",
orientation: "manual",
material: "acrylic_uvt_good",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
drawstyle: "solid",
}
{
name: "GEO",
index: "holder_ring_config14_scan_side_ring_4_1",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tubearray",
r_max: 134.0,
r_min: 107.0,
size_z: 3.1,
pos_table: "RINGINFO_config14_scan_side_ring_4_1",
enable: 1,
orientation: "manual",
material: "acrylic_uvt_good",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
drawstyle: "solid",
}
{
name: "GEO",
index: "holder_ring_config14_scan_side_ring_4_2",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tubearray",
r_max: 134.0,
r_min: 107.0,
size_z: 3.1,
pos_table: "RINGINFO_config14_scan_side_ring_4_2",
enable: 1,
orientation: "manual",
material: "acrylic_uvt_good",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
drawstyle: "solid",
}
{
name: "GEO",
index: "holder_ring_config14_scan_side_ring_6",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tubearray",
r_max: 134.0,
r_min: 107.0,
size_z: 3.1,
pos_table: "RINGINFO_config14_scan_side_ring_6",
orientation: "manual",
material: "acrylic_uvt_good",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 0,
drawstyle: "solid",
}

/////////////////////////////////////////////////////////////////////
///////////*** End of Watchman/boy holder rings ***//////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////******** Inner structures from WCSim ********///////////////
/////////////////////////////////////////////////////////////////////

{
name: "GEO",
index: "InnerStructure_Holders_Blacksheets",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
// enable_inner_structure: 0 => no inner structure
// enable_inner_structure: 1 => inner structure made from primitives based on gdml
// enable_inner_structure: 2 => inner structure loaded from gdml
enable_inner_structure: 1, 
inner_structure_visualise: 0, // when enable_inner_structure: 1 is set. 
inner_structure_gdml_file: "../share/annie/ratdb/ANNIE/annie_phase2_structure.gdml",
inner_structure_center: [0.0, 0.0, -1968.0],
inner_structure_rotation_angle: 67.5, // to rotate the structure along the Z (vertical) axis. 67.5° from stl
inner_structure_wrapper_material: "tyvek_wrapper_ANNIE", //"tyvek", //could be something different, which is defined in the OPTICS_ANNIE.ratdb
inner_structure_color: [1.0, 0.0, 0.0, 0.0],
inner_structure_invisible: 0,
enable_sandi_configuration: 1,
pmt_position_file:"../share/annie/ratdb/ANNIE/PMTPositions_Scan_Glass.txt",
enable_annie_holders: 1, //ANNIE holders on the side; code copied from WCSim 
annie_holders_color: [1.0, 1.0, 1.0, 0.5],
annie_holders_invisible: 0,
enable_luxetel_holders: 1, //ANNIE holders on the top and bottom; code copied from WCSim 
luxetel_holders_color: [1.0, 1.0, 1.0, 0.5],
luxetel_holders_invisible: 0,
enable_black_sheets: 1, //Black sheet for optical insulation; octagonal shape; code copied from WCSim
black_sheet_color: [0.0, 1.0, 0.0, 1.0],
black_sheet_invisible: 1,
write_gdml: 0, //Write a gdml file to check the geometry by eye with a CAD program
enable_superSANDI: 0,
gdml_out_file: "test_output.gdml",
type: "annieInnerStructures", //see the geo factory
}

/////////////////////////////////////////////////////////////////////
//////////******** End of inner structures ********//////////////////
/////////////////////////////////////////////////////////////////////
