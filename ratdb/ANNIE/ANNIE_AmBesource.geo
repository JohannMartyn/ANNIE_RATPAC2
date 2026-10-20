////// AmBe source geometry
// The source (beryllium) is enclosed in a case (stainless steel)

// Beam direction in Data: +Z, Tank height axis: Y
// Beam direction in MC: +Y, Tank height axis: Z

// Housing: PVC cup (body) with a 12.5 mm bottom wall and an open top; the bore (air)
// is flush with the body top. The lowest 15 mm of the cap (plug) sits in the top of
// the bore. The rest of the cap (flange + cone) is a daughter of the detector and
// rests on body, bore and plug without overlapping them.

{
name: "GEO",
index: "AmBe_housing_body",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "tube",
r_max: 35.0,
size_z: 62.25, // 124.5 mm long: former 12.5 mm top wall removed, the cap closes the top
position: [0.0, 0.0, 554.5],
//Center (AmBe source not housing) is [0.0, 0.0, -46.75] from this position = [0.0, 0.0, 7.7] in the mother (unchanged), Axis are [+MITPC/-stairs;+FMV/-MRD;+top/-bottom]
material: "pvc_white",
surface: "ptfe",
color: [1.0, 0.0, 0.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_housing_air",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_body",
type: "tube",
r_max: 25.0,
size_z: 56.0,
position: [0.0, 0.0, 6.25], // 62.25 - 56.0: top flush with the body top; still centred at 60.7 in the mother
material: "pvc_white",
color: [0.0, 1.0, 0.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_housing_cap", //cap of the housing above the body: 5 mm flange + 25 mm cone
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "detector",
type: "revolve",
r_max: [35.0, 35.0, 20.0],
r_min: [0.0, 0.0, 0.0],
z: [0.0, 5.0, 30.0],
position: [0.0, 0.0, 624.4], // on the body top (54.45 + 62.25)
material: "acrylic_uvt",
surface: "ptfe",
color: [0.0, 0.0, 1.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_housing_cap_plug", //lowest 15 mm of the cap, inserted in the top of the bore
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_air",
type: "tube",
r_max: 25.0,
size_z: 7.5,
position: [0.0, 0.0, 48.5], // 56.0 - 7.5: top flush with the air top
material: "acrylic_uvt",
surface: "ptfe",
color: [0.0, 0.0, 1.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_BGO",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_air",
type: "tube",
r_max: 25.0,
size_z: 25.0,
position: [0.0, 0.0, -20.0],
material: "BGO_scint",
color: [1.0, 1.0, 0.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_placer",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_housing_air",
type: "tube",
r_max: 25.0,
size_z: 5.5,
position: [0.0, 0.0, -50.5],
material: "acrylic_uva_McMaster",
color: [0.0, 1.0, 1.0, 0.5],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBe_case",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_placer",
type: "tube",
r_max: 2.5,
size_z: 2.5,
position: [0.0, 0.0, -2.5],
material: "stainless_steel",
color: [0.0, 1.0, 1.0, 0.7],
drawstyle: "solid",
}

{
name: "GEO",
index: "AmBeSource",
enable: 1,
valid_begin: [0, 0],
valid_end: [0, 0],
mother: "AmBe_case",
type: "tube",
r_max: 0.5,
size_z: 0.5,
position: [0.0, 0.0, 0.0],
material: "beryllium",
color: [0.0, 1.0, 1.0, 0.9],
drawstyle: "solid",
}
