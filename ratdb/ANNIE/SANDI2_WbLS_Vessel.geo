/////////////////////////////////////////////////////////////////////
///////////////********** WbLS volumes ***********////////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "wbls_vessel",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 450.0,
r_max: 380.0,
position: [0.0,0.0,0.0],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid",
}

{
name: "GEO",
index: "wblsvolume_liquid",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "wbls_vessel",
type: "tube",
size_z: 450.0,
r_max: 360.0,
position: [0.0,0.0,0.0],
material: "wbls1pct_ly95_gdS0p2pct_SANDI",
color: [0.9, 0.1, 0.4, 0.9],
drawstyle: "solid", 
}

{
name: "GEO",
index: "wbls_vessel_topcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 12.7,
r_min: 0.0,
r_max: 380.0,
position:  [0.0,0.0,462.7],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
}

{
name: "GEO",
index: "wbls_vessel_topcap_lip",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 25.4,
r_min: 380.0,
r_max: 460.0,
position:  [0.0,0.0,450.0],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
}


{
name: "GEO",
index: "wbls_vessel_botcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 12.7,
r_min: 0.0,
r_max: 460.0,
position:  [0.0,0.0,-512.7],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
}

{
name: "GEO",
index: "wbls_basket_cable1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 699.7,
r_min: 0.0,
r_max: 6.35,
position: [0.0, 0.0, 1275.1],
material: "pvc_white",
color: [1.0, 0.5, 0.5, 0.1],
drawstyle: "solid",
invisible: 0,
}

{
name: "GEO",
index: "wbls_basket_cable2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
size_z: 749.7,
r_min: 0.0,
r_max: 6.35,
position: [314.66, 0.0, 1225.1],
material: "pvc_white",
color: [1.0, 0.5, 0.5, 0.1],
drawstyle: "solid",
invisible: 0,
}

{
name: "GEO",
index: "wbls_vessel_oring",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "wbls_vessel_topcap_lip",
type: "tube",
size_z: 3.5,
r_min: 380.0,
r_max: 387.0,
position: [0.0, 0.0,0.0],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "polyethylene_black",
color: [1.0, 0.5, 0.5, 0.4],
drawstyle: "solid",
}

{
name: "wbls_bridal_bars_table",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [300.98, 300.98, -300.98, -300.98],
y: [300.98, -300.98, -300.98, 300.98],
z: [0.0, 0.0, 0.0, 0.0],
dir_x: [0.0, 0.0, 0.0, 0.0],
dir_y: [0.0, 0.0, 0.0, 0.0],
dir_z: [1.0, 1.0, 1.0, 1.0],
}

{ 
name: "GEO", 
index: "wbls_bridal_bars", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "tubearray",  
size_z: 450.0,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_bridal_bars_table", 
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "wbls_bridal_bars_table_topbot",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [300.98, 300.98, -300.98, -300.98],
y: [300.98, -300.98, -300.98, 300.98],
z: [0.0, 0.0, 0.0, 0.0],
dir_x: [0.0, 0.0, 0.0, 0.0],
dir_y: [0.0, 0.0, 0.0, 0.0],
dir_z: [1.0, 1.0, 1.0, 1.0],
}

{ 
name: "GEO", 
index: "wbls_bridal_bars_top", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "wbls_vessel_topcap_lip", 
type: "tubearray",  
size_z: 25.4,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_bridal_bars_table_topbot",
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{ 
name: "GEO", 
index: "wbls_bridal_bars_bot", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "wbls_vessel_botcap", 
type: "tubearray",  
size_z: 12.7,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_bridal_bars_table_topbot",
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "wbls_bridal_bars_table_topbot_caps",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [300.98, 300.98, -300.98, -300.98, 300.98, 300.98, -300.98, -300.98],
y: [300.98, -300.98, -300.98, 300.98, 300.98, -300.98, -300.98, 300.98],
z: [560.4, 560.4, 560.4, 560.4, -560.4, -560.4, -560.4, -560.4],
dir_x: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_y: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_z: [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
}

{ 
name: "GEO", 
index: "wbls_bridal_bars_topbot_caps", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "tubearray",  
size_z: 5.0,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_bridal_bars_table_topbot_caps",
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "wbls_vessel_screws_pos_table_middle",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [81.94, 233.34, 349.22, 411.93, 411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94],
y: [411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94, 81.94, 233.34, 349.22, 411.93],
z: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_x: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_y: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_z: [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
}

{
name: "wbls_vessel_screws_pos_table_topbot",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [81.94, 233.34, 349.22, 411.93, 411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94, 81.94, 233.34, 349.22, 411.93, 411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94],
y: [411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94, 81.94, 233.34, 349.22, 411.93, 411.93, 349.22, 233.34, 81.94, -81.94, -233.34, -349.22, -411.93, -411.93, -349.22, -233.34, -81.94, 81.94, 233.34, 349.22, 411.93],
z: [424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 424.1, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9, 475.9],
dir_x: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_y: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
dir_z: [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
}


{ 
name: "GEO", 
index: "wbls_vessel_screws_pos_table_middle", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "wbls_vessel_topcap_lip", 
type: "tubearray",  
size_z: 50.8,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_vessel_screws_pos_table_middle", //generated on the fly
// start_idx: 70, //index of first inner pmt
// end_idx: 79, //index of last inner pmt
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{ 
name: "GEO", 
index: "wbls_vessel_screws_topbot", 
valid_begin: [0, 0], 
valid_end: [0, 0], 
mother: "detector", 
type: "tubearray",  
size_z: 10.0,
r_min: 0.0,
r_max: 6.35,
pos_table: "wbls_vessel_screws_pos_table_topbot", //generated on the fly
// start_idx: 70, //index of first inner pmt
// end_idx: 79, //index of last inner pmt
orientation: "manual",
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
} 

{
name: "GEO",
index: "wbls_basket_metalbeam_top1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [314.66, 12.7, 40.0], // mm
position: [0.0, -301.96, 515.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_top2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [314.66, 12.7, 40.0], // mm
position: [0.0, 301.96, 515.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_top3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [12.7, 301.96, 50.0], // mm
position: [0.0, 0.0, 525.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_top4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [6.35, 20.0, 20.0], // mm
position: [0.0, 0.0, 595.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_bot1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [314.66, 314.66, 40.0], // mm
position: [0.0, 0.0, -515.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_bot2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "wbls_basket_metalbeam_bot1",
type: "box",
size: [289.26, 289.26, 40.0], // mm
position: [0.0, 0.0, 0.0],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "water_gdS_0p2",
color: [0.6, 0.8, 1.0, 0.1],
drawstyle: "solid",
invisible: 0,
}
/////////////////////////////////////////////////////////////////////
////////////********** End of WbLS volumes ***********////////////////
/////////////////////////////////////////////////////////////////////

