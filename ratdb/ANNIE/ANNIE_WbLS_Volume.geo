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
size_z: 430.0,
r_max: 440.0,
position: [0.0,0.0,0.0],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "wbls_vessel_topcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [460.0, 460.0, 12.7], // mm
position:  [0.0,0.0,0.0],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "wbls_vessel_botcap",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [460.0, 460.0, 12.7], // mm
position: [0.0,0.0,0.0],
material: "acrylic_uvt",
color: [0.1, 0.4, 0.6, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "wblsvolume_liquid",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "wbls_vessel",
type: "tube",
r_max: 430.0,
size_z: 430.0,
position: [0.0,0.0,0.0],
material: "wbls_1pct_0420",
color: [0.2, 0.1, 0.4, 0.3],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_bot1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [2.5, 608.1, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_bot2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [2.5, 608.1, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_bot3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [608.1, 2.5, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_bot4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [608.1, 2.5, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_top1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [2.5, 608.1, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid", 
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_top2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [2.5, 608.1, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_top3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [608.1, 2.5, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_basket_metalplate_top4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "box",
size: [608.1,2.5, 19.05], // mm
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_bridal_bar1",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 430.0,
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_bridal_bar2",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 430.0,
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_bridal_bar3",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 430.0,
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}

{
name: "GEO",
index: "wbls_bridal_bar4",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 6.35,
size_z: 430.0,
position: [0.0,0.0,0.0],
material: "stainless_steel",
color: [0.1, 0.4, 0.1, 0.8],
drawstyle: "solid",
enable: 1,
}
/////////////////////////////////////////////////////////////////////
////////////********** End of WbLS volumes ***********////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
///////////////********** WbLS factory ***********/////////////////////
/////////////////////////////////////////////////////////////////////
{
name: "GEO",
index: "WbLSVolume",
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
enable_wblsvolume: 1,
wblsvolume_center: [0.0, 0.0, 0.0], // position WRT the centre of the detector
type: "annieWbLSVolume", //see the geo factory
}
/////////////////////////////////////////////////////////////////////
/////////********** End of WbLS factory ***********/////////////
/////////////////////////////////////////////////////////////////////
