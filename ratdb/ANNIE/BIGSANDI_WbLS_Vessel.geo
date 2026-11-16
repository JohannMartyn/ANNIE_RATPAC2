/////////////////////////////////////////////////////////////////////
///////////////********** WbLS volumes ***********////////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "wbls_vessel",
run_range: [0, 0],
mother: "detector",
type: "tube",
size_z: 1400.0,
r_max: 925.0,
position: [0.0,0.0,-33.8],
material: "acrylic_SANDI",
color: [0.1, 0.4, 0.6, 1.0],
drawstyle: "solid",
}

{
name: "GEO",
index: "wblsvolume_liquid",
run_range: [0, 0],
mother: "wbls_vessel",
type: "tube",
size_z: 1399.9,
r_max: 924.9,
position: [0.0,0.0,0.0],
material: "wbls1pct_ly95_gdS0p2pct_SANDI",
color: [0.9, 0.1, 0.4, 1.0],
drawstyle: "solid", 
}

/////////////////////////////////////////////////////////////////////
////////////********** End of WbLS volumes ***********////////////////
/////////////////////////////////////////////////////////////////////

