/////////////////////////////////////////////////////////////////////
///////////////********** NCV volumes ***********////////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "ncv_vessel",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 254.0,
r_max: 254.0,
r_min: 250.25,
// low_norm: [0.0, 0.0, -1.0],
// high_norm: [0.0, 0.0, 1.0],
//position: [0.0, 0.0, 0.0],
position: [793.7, 0.0, 1609.7],
material: "acrylic_uva_McMaster",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_vessel_topcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [254.0, 254.0, 12.7], // mm
//position: [0.0, 0.0, 0.0],
position: [793.7, 0.0, 1876.4],
material: "acrylic_uva_McMaster",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_vessel_botcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [254.0, 254.0, 12.7], // mm
//position: [0.0, 0.0, 0.0],
position: [793.7, 0.0, 1343.0],
material: "acrylic_uva_McMaster",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_liquid",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 250.25,
size_z: 254.0,
//position: [0.0, 0.0, 0.0],
position: [793.7, 0.0, 1609.7],
material: "ej335",
color: [0.2, 0.1, 0.4, 0.3],
drawstyle: "solid", 
enable: 1,
}

// First NCV PMT
{
name: "GEO",
index: "ncv_pmt_vessel_1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 38.7,
size_z: 150.0,
position: [893.7, 0.0, 2039.1],
material: "acrylic_uva_McMaster",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_pmt_vessel_blacksheet_1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 150.0,
r_max: 38.71, // 10 microns thick (about right)
r_min: 38.7,
// low_norm: [0.0, 0.0, -1.0],
// high_norm: [0.0, 0.0, 1.0],
//position: [0.0, 0.0, 0.0],
position: [893.7, 0.0, 2039.1],
material: "acrylic_black", //not true but black 
color: [1.0, 0.2, 0.6, 1.0],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_pmt_vessel_air_1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "ncv_pmt_vessel_1",
type: "tube",
r_max: 38.2,
size_z: 148.0,
position: [0.0, 0.0, 0.0],
material: "air",
color: [0.2, 0.1, 0.4, 0.3],
drawstyle: "solid", 
enable: 1,
}

// Second NCV PMT
{
name: "GEO",
index: "ncv_pmt_vessel_2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 38.7,
size_z: 150.0,
position: [693.7, 0.0, 2039.1],
material: "acrylic_uva_McMaster",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_pmt_vessel_blacksheet_2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 150.0,
r_max: 38.71, // 10 microns thick (about right)
r_min: 38.7,
// low_norm: [0.0, 0.0, -1.0],
// high_norm: [0.0, 0.0, 1.0],
//position: [0.0, 0.0, 0.0],
position: [693.7, 0.0, 2039.1],
material: "acrylic_black", //not true but black 
color: [1.0, 0.2, 0.6, 1.0],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_pmt_vessel_air_2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "ncv_pmt_vessel_2",
type: "tube",
r_max: 38.2,
size_z: 148.0,
position: [0.0, 0.0, 0.0],
material: "air",
color: [0.2, 0.1, 0.4, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_bot1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, 190.13, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_bot2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, -190.13, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_bot3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, -207.83, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_bot4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, 207.83, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_top1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, 190.13, 1908.15],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_top2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, -190.13, 1908.15],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_top3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, -207.83, 1908.15],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_top4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [190.0, 2.5, 19.05], // mm
position: [793.7, 207.83, 1908.15],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_ballast1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 19.05,
size_z: 187.63,
position: [603.57, 0.0, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
rotation: [90.0, 0.0, 0.0]
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_ballast2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 19.05,
size_z: 187.63,
position: [983.83, 0.0, 1311.25],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
rotation: [90.0, 0.0, 0.0]
enable: 1,
}

{
name: "GEO",
index: "ncv_basket_metalplate_topcenter",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [2.5, 187.63, 25.4], // mm
position: [793.7, 0.0, 1914.5],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_bridal_bar1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 254.0,
position: [981.33, 187.63, 1609.7],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_bridal_bar2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 254.0,
position: [981.33, -187.63, 1609.7],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_bridal_bar3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 254.0,
position: [606.07, 187.63, 1609.7],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "ncv_bridal_bar4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 254.0,
position: [606.07, -187.63, 1609.7],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}
/////////////////////////////////////////////////////////////////////
////////////********** End of NCV volumes ***********////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
////////////********** NCV PMTs ***********////////////////
/////////////////////////////////////////////////////////////////////
{ 
name: "GEO", 
index: "ncv_pmt_1",
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "ncv_pmt_vessel_air_1", 
type: "pmtarray", 
pmt_model: "b76d01",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
efficiency_correction: 1.000,  
pos_table: "PMTINFO_run1_ncvpmt_1", //generated on the fly
// start_idx: 60, //index of first inner pmt
// end_idx: 1, //index of last inner pmt
orientation: "manual",
enable: 1,
} 

{ 
name: "GEO", 
index: "ncv_pmt_2",
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "ncv_pmt_vessel_air_2", 
type: "pmtarray", 
pmt_model: "b76d01",
pmt_detector_type: "idpmt",
sensitive_detector: "/mydet/pmt/inner", 
efficiency_correction: 1.000,  
pos_table: "PMTINFO_run1_ncvpmt_2", //generated on the fly
// start_idx: 60, //index of first inner pmt
// end_idx: 1, //index of last inner pmt
orientation: "manual",
enable: 1,
}
/////////////////////////////////////////////////////////////////////
////////////********** End of NCV PMTs ***********////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
///////////////********** NCV factory ***********/////////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "NCV",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
enable_ncv: 1,
// ncv_center: [793.7, 0.0, 1609.7], // top upstream position (May 2016 to 06-02-2017) (in dd-mm-yyyy system)
// ncv_center: [1117.55, 0.0, -562.0], // bottom upstream position (06-02-2017 to 27-02-2017)
// ncv_center: [-146.1, 0.0, 1597.18], // position 1 (top center) (27-02-2017 to 14-04-2017) 
//ncv_center: [-146.1, 0.0, 276.38], // position 2 (center center) (14-04-2017 to 24-05-2017)
// ncv_center: [0.0, 0.0, 0.0], // position 2 (center center) (14-04-2017 to 24-05-2017)
 ncv_center: [793.7, 0.0, 276.38], // position 3 (center upstream) (24-05-2017 to 08-06-2017)
// ncv_center: [-146.1, 0.0, 1292.33], // position 4 (top center minus 1 foot) (08-06-2017 to 23-06-2017)
// ncv_center: [-146.1, 0.0, 987.53], // position 5 (top center minus 2 feet) (23-06-2017 to 30-06-2017)
// ncv_center: [-146.1, 0.0, 1444.78], // position 6 (top center minus 1/2 feet) (30-06-2017 to 03-07-2017)
// ncv_center: [311.1, 0.0, 276.38], // position 7 (between 2 and 3) (03-07-2017 to 19-07-2017)
// ncv_center: [311.1, 0.0, 1597.18], // position 8 (height of 1, horizontal same as 7) (19-07-2017 to ??)
type: "annieNCV", //see the geo factory
}
/////////////////////////////////////////////////////////////////////
/////////********** End of NCV factory ***********/////////////
/////////////////////////////////////////////////////////////////////
