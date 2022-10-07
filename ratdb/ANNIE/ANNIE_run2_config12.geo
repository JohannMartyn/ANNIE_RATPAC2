////////////////////////////////////////////////////////
// ANNIE detector geometry for Phase 2 
// 
// Config 12: - 20 LUX 10inch PMTs at bottom 
//	     - 48 10inch Watchboy, 40 8inch hqe, 4 10inch HQE Watchman
//           - 20 11 inch ETEL PMT on top 
//
// Author: V. Fischer <vfischer@ucdavis.edu>
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
size: [3910.0, 4250.0, 2845.0], // mm, half-length
position: [0.0, 2235.0, 2819.6],
color: [0.5, 1.0, 0.0, 0.1],
material: "air",
invisible: 1,
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
position: [0.0, -2268.8, -1070.2],
material: "stainless_steel",
color: [1.0, 0.0, 0.0, 0.1],
invisible: 1,
drawstyle: "solid",
rotation: [90.0, 0.0, 00.0],
}

{
name: "GEO",
index: "liner",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "tank",
type: "tube",
surface: "pvc_white",
r_max: 1517.6,
size_z: 1974.8,
position: [0.0, 0.0, 0.0],
material: "pvc_white",
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
color: [0.6, 0.8, 1.0, 0.9],
drawstyle: "solid",
}

{
name: "GEO",
index: "optical_isolation_barrel",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
surface: "polyethylene_black",
r_max: 1417.6,
r_min: 1416.6,
size_z: 1704.8,
position: [0.0, 0.0, 0.0],
material: "polyethylene_black",
color: [1.0, 0.5, 0.5, 0.1],
// invisible: 0,
drawstyle: "solid",
}

{
name: "GEO",
index: "optical_isolation_top",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
surface: "polyethylene_black",
r_max: 1417.6,
size_z: 0.5,
position: [0.0, 0.0, 1704.8],
material: "polyethylene_black",
color: [1.0, 0.5, 0.5, 0.1],
// invisible: 0,
drawstyle: "solid",
}

{
name: "GEO",
index: "optical_isolation_bottom",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
surface: "polyethylene_black",
r_max: 1417.6,
size_z: 0.5,
position: [0.0, 0.0, -1704.8],
material: "polyethylene_black",
color: [1.0, 0.5, 0.5, 0.1],
// invisible: 0,
drawstyle: "solid",
}

// {
// name: "GEO",
// index: "detector_fiducial",
// valid_begin: [0, 0],
// valid_end: [0, 0],
// mother: "detector",
// type: "tube",
// //r_max: 1150.0,
// r_max: 1000.0,
// //size_z: 1500.0,
// size_z: 1450.0,
// position: [0.0, 0.0, 0.0],
// material: "water_gdS_0p2",
// color: [0.1, 0.4, 0.6, 0.9],
// drawstyle: "solid"
// }

// {
// name: "GEO",
// index: "detector_fiducial_muon",
// valid_begin: [0, 0],
// valid_end: [0, 0],
// mother: "detector",
// type: "box",
// size: [1000.0, 600.0, 500], // mm
// position: [600.0, 0.0, 0.0],
// material: "water_gdS_0p2",
// color: [0.6, 0.4, 0.6, 0.5],
// drawstyle: "solid",
// rotation: [0.0, 90.0, 0.0],
// }
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_bottomgrid", //generated on the fly
// start_idx: 0, //index of first inner pmt
// end_idx: 19, //index of last inner pmt
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_topgrid", //generated on the fly
// start_idx: 20, //index of first inner pmt
// end_idx: 41, //index of last inner pmt
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_1", //generated on the fly
// start_idx: 60, //index of first inner pmt
// end_idx: 69, //index of last inner pmt
orientation: "manual",
//orient_point: [1.0, -33.8, -1054.8],
// orient_point: [0.0, 0.0, -1054.8],
// orient_point: [0.0, 0.0, -133.3], 
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_2_1", //generated on the fly
// start_idx: 70, //index of first inner pmt
// end_idx: 79, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, -621.3],
// orient_point: [0.0, 0.0, -621.3],
// orient_point: [0.0, 0.0, -133.3], 
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_2_2", //generated on the fly
// start_idx: 70, //index of first inner pmt
// end_idx: 79, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, -621.3],
// orient_point: [0.0, 0.0, -621.3],
// orient_point: [0.0, 0.0, -133.3], 
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_3", //generated on the fly
// start_idx: 80, //index of first inner pmt
// end_idx: 89, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, -187.9],
// orient_point: [0.0, 0.0, -187.9],
// orient_point: [0.0, 0.0, -133.3],   
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_4_1", //generated on the fly
// start_idx: 90, //index of first inner pmt
// end_idx: 99, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, 245.6],
// orient_point: [0.0, 0.0, 245.6],
// orient_point: [0.0, 0.0, -133.3],  
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_4_2", //generated on the fly
// start_idx: 90, //index of first inner pmt
// end_idx: 99, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, 245.6],
// orient_point: [0.0, 0.0, 245.6],
// orient_point: [0.0, 0.0, -133.3],  
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_5", //generated on the fly
// start_idx: 100, //index of first inner pmt
// end_idx: 109, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, 679.0],
// orient_point: [0.0, 0.0, 679.0],
// orient_point: [0.0, 0.0, -133.3],  
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
efficiency_correction: 1.000,  
pos_table: "PMTINFO_config12_side_ring_6", //generated on the fly
// start_idx: 100, //index of first inner pmt
// end_idx: 109, //index of last inner pmt
orientation: "manual",
// orient_point: [1.0, -33.8, 1112.5],
// orient_point: [0.0, 0.0, 1112.5],
// orient_point: [0.0, 0.0, -133.3],  
}

/////////////////////////////////////////////////////////////////////
///////////********** End of PMT arrays ***********//////////////////
/////////////////////////////////////////////////////////////////////


