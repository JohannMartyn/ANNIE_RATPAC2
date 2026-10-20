/////////////////////////////////////////////////////////////////////
///////////////********** WbLS volumes ***********////////////////////
/////////////////////////////////////////////////////////////////////
// Overlap-free version :-D
// Conventions (RAT-PAC / Geant4): tube size_z and box size are HALF-lengths, units mm.
//
// Resulting z-stack (absolute, mm):
//   bridal bar end caps (top)   555.4 ..  565.4
//   top beams top1/top2/top3    475.4 ..  555.4   (top3_upper 555.4..575.4, top4 575.4..615.4)
//   topcap (r<380)/lip (380-460) 450.0 / 424.6 ..  475.4
//   vessel + liquid            -450.0 ..  450.0
//   botcap (r<460)             -475.4 .. -450.0
//   bottom beam frame          -555.4 .. -475.4
//   bridal bar end caps (bot)  -565.4 .. -555.4
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
material: "acrylic_SANDI",
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
position:  [0.0,0.0,462.7],   // z: 450.0 .. 475.4
material: "acrylic_SANDI",
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
position:  [0.0,0.0,450.0],   // z: 424.6 .. 475.4
material: "acrylic_SANDI",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
}


// FIX: was z = -512.7 (cap spanned -525.4..-500.0, overlapping the bottom beam frame
// and leaving a 50 mm gap below the vessel). Moved to mirror the top cap:
// cap now spans -475.4..-450.0, sitting directly under the vessel and on top of the
// bottom beam frame (-555.4..-475.4), exactly as the top cap sits under the top beams.
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
position:  [0.0,0.0,-462.7],  // z: -475.4 .. -450.0
material: "acrylic_SANDI",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
}

//{
//name: "GEO",
//index: "wbls_basket_cable1",
//valid_begin: [0, 0],
//valid_end: [0, 0],
//mother: "detector",
//type: "tube",
//size_z: 699.7,
//r_min: 0.0,
//r_max: 6.35,
//position: [0.0, 0.0, 1275.1],
//material: "pvc_white",
//color: [1.0, 0.5, 0.5, 0.1],
//drawstyle: "solid",
//invisible: 0,
//}

//{
//name: "GEO",
//index: "wbls_basket_cable2",
//valid_begin: [0, 0],
//valid_end: [0, 0],
//mother: "detector",
//type: "tube",
//size_z: 749.7,
//r_min: 0.0,
//r_max: 6.35,
//position: [314.66, 0.0, 1225.1],
//material: "pvc_white",
//color: [1.0, 0.5, 0.5, 0.1],
//drawstyle: "solid",
//invisible: 0,
//}

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

/////////////////////////////////////////////////////////////////////
// Bridal bars (threaded rods at r = 425.6 mm)
/////////////////////////////////////////////////////////////////////

// FIX: main bar segment previously spanned -450..450 and ran 25.4 mm into the
// top-cap lip (424.6..475.4). The part inside the lip is already modelled by
// wbls_bridal_bars_top (daughter of the lip), so the main segment now stops at
// the lip's lower face: z = -450.0 .. 424.6 (half-length 437.3, centre -12.7).
{
name: "wbls_bridal_bars_table",
valid_begin: [0, 0],
valid_end: [0, 0],
x: [300.98, 300.98, -300.98, -300.98],
y: [300.98, -300.98, -300.98, 300.98],
z: [-12.7, -12.7, -12.7, -12.7],
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
size_z: 437.3,                // z: -450.0 .. 424.6
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

// Bar segment inside the top-cap lip (z 424.6 .. 475.4)
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

// Bar segment inside the bottom cap (z -475.4 .. -450.0)
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

// Bar end caps: sit on top of the top beams (555.4) and under the bottom frame (-555.4)
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

/////////////////////////////////////////////////////////////////////
// Lip screws (16 x, r = 420 mm). Total screw length 101.6 mm, z 399.2 .. 500.8
/////////////////////////////////////////////////////////////////////
// FIX: the original "middle" screws (half-length 50.8) were daughters of the lip
// but 2x as long as the lip, protruding 25.4 mm out of their mother on both sides,
// and the "topbot" pieces (r = 6.35, z 414.1..434.1 and 465.9..485.9) overlapped
// the lip by 9.5 mm. Since both have the same radius, the topbot pieces lay fully
// inside the protruding screw shafts. The same total steel volume is now split
// into three non-overlapping pieces per screw:
//   - inside the lip (daughter of lip):  z 424.6 .. 475.4  (half 25.4)
//   - below the lip  (in detector):      z 399.2 .. 424.6  (half 12.7, centre 411.9)
//   - above the lip  (in detector):      z 475.4 .. 500.8  (half 12.7, centre 488.1)

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
z: [411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 411.9, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1, 488.1],
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
size_z: 25.4,                 // was 50.8; now exactly the lip thickness
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
size_z: 12.7,                 // was 10.0; covers the screw ends outside the lip
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

/////////////////////////////////////////////////////////////////////
// Basket beams
/////////////////////////////////////////////////////////////////////

{
name: "GEO",
index: "wbls_basket_metalbeam_top1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [314.66, 12.7, 40.0], // mm (half-lengths); y: -314.66 .. -289.26, z: 475.4 .. 555.4
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
size: [314.66, 12.7, 40.0], // mm (half-lengths); y: 289.26 .. 314.66, z: 475.4 .. 555.4
position: [0.0, 301.96, 515.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

// FIX: the cross beam (y -301.96..301.96, z 475.4..575.4) ran 12.7 mm into
// top1/top2 at both ends. It is split into two boxes with the same outer shape:
//   lower part (z 475.4..555.4): between the inner faces of top1/top2, |y| <= 289.26
//   upper part (z 555.4..575.4): above top1/top2, full original length |y| <= 301.96
{
name: "GEO",
index: "wbls_basket_metalbeam_top3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [12.7, 289.26, 40.0], // mm (half-lengths); z: 475.4 .. 555.4
position: [0.0, 0.0, 515.4],
rotation: [0.0, 0.0, 0.0], // X, Y, Z axis rotations (deg) of element about its center. Rotations are applied in X, Y, Z order
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
}

{
name: "GEO",
index: "wbls_basket_metalbeam_top3_upper",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [12.7, 301.96, 10.0], // mm (half-lengths); z: 555.4 .. 575.4
position: [0.0, 0.0, 565.4],
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
size: [6.35, 20.0, 20.0], // mm (half-lengths); z: 575.4 .. 615.4
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
size: [314.66, 314.66, 40.0], // mm (half-lengths); z: -555.4 .. -475.4 (now directly under botcap)
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


