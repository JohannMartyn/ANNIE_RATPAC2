// ---------------------------------------------------------------------------
//  pmtinfo_to_ratdb.cpp
//
//  Convert the PMT laser scan table of the form
//
//      <PMT ID> <panel> <x> <y> <z> <dir_x> <dir_y> <dir_z> <PMT type>
//
//  into RAT-PAC PMTINFO .ratdb tables. You must use the glass scan!
//
//  The block layout in kBlocks below (config14 scan geometry).  
//  Positions are looked up per channel number in the input table,
//  so the name <-> channel_number association is fixed by this file and cannot
//  drift.
//
//  NOTE: the dir_x/dir_y/dir_z columns of the input table are IGNORED.
//  Directions are recomputed exactly from the positions instead - see makeDir():
//  THis is how it was done in WCSim also.
//      bottom grid (panel 0) -> (0, 0, +1)   looks up
//      top    grid (panel 9) -> (0, 0, -1)   looks down
//      side panels (1 .. 8)  -> horizontal, aimed at the tank center axis
//
//  build : g++ -std=c++11 -O2 -o pmtinfo_to_ratdb pmtinfo_to_ratdb.cpp
//  run   : ./pmtinfo_to_ratdb pmt_table.txt [output.ratdb]
//          (writes to stdout if no output file is given)
// ---------------------------------------------------------------------------

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// IDs of the Watchman and Watchboy PMTs
static const int WatchmanboyIDs[49] = {
  382, 393, 404, 405, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 462, 463
};

// settings ------------------------------------------------------------------
static const double POS_SCALE = 10.0;  // e.g. 10.0 to go from cm to mm
static const int    PRECISION = 6;     // significant digits, positions
static const int    DIR_PREC  = 9;     // significant digits, direction cos

// coordinate system definition ---------------------------------------------- 
// All lengths below are in mm, i.e. AFTER POS_SCALE has been applied.
//
// Data frame (input table): y is vertical, the tank axis runs along y at
//                           (x, z) = (DATA_AXIS_X, DATA_AXIS_Z).
// RATDB frame  (output):    z is vertical, the tank axis runs along z at
//                           (x, y) = (TANK_AXIS_X,  TANK_AXIS_Y).
//
static const double DATA_AXIS_X = 0.0; // tank axis in the data frame
static const double DATA_AXIS_Z = 1681.0; // (168.10 cm; circle fit: 167.95)
static const double TANK_AXIS_X   = 0.0; // where that axis sits in RATDB
static const double TANK_AXIS_Y   = 1198.6; // Offset from the mother volume positions
static const double VERT_OFFSET   = 144.5 - 3339.0;  // data y -> RATDB z

// Panel numbers (column 2 of the input table) that mark the two flat grids.
static const int PANEL_BOTTOM = 0;
static const int PANEL_TOP    = 9;

// ------------------------------------------------------------------- types
struct PMT {
  int    id   = 0;
  int    row  = 0;
  // column 2: panel number.  0 = bottom grid, 9 = top grid,
  // 1...8 = the eight vertical side racks.
  double x    = 0.0, y  = 0.0, z  = 0.0;
  double dx   = 0.0, dy = 0.0, dz = 0.0;   // read but not used, see makeDir()
  int    type = 0;
};

struct Block {
  std::string      name;      // goes into  name: "..."
  std::string      comment;   // the //<pmt model> <type> line, copied as is
  std::vector<int> channels;  // channel_number list
};

// hard-coded layout ------------------------------------------------------- 
// Edit here if the grouping ever changes.  A channel may appear in more than
// one block (topgrid / topgrid_SANDI share theirs). That is fine.
static const Block kBlocks[] = {
    {"PMTINFO_config14_scan_bottomgrid",
     "//r7081_ANNIE 0",
     {332, 333, 334, 335, 336, 337, 338, 339, 340, 341,
      342, 343, 344, 345, 346, 347, 348, 349, 350, 351}},

    {"PMTINFO_config14_scan_side_ring_1",
     "//r7081_ANNIE 0",
     {417, 418, 427, 429, 431, 433, 434, 436, 444, 452, 453, 454}},

    {"PMTINFO_config14_scan_side_ring_2_1",
     "//r5912_hqe_ANNIE 2",
     {374, 379, 385, 392, 395, 399, 408, 414}},

    {"PMTINFO_config14_scan_side_ring_2_2",
     "//r7081_ANNIE 0",
     {419, 420, 432, 439, 445, 447, 449, 458}},

    {"PMTINFO_config14_scan_side_ring_3",
     "//r5912_hqe_ANNIE 2",
     {376, 377, 378, 381, 387, 388, 390, 391,
      394, 396, 401, 402, 406, 407, 409, 412}},

    {"PMTINFO_config14_scan_side_ring_4_1",
     "//r7081_ANNIE 0",
     {421, 424, 425, 428, 435, 440, 441, 450, 451, 457, 462, 463}},

    {"PMTINFO_config14_scan_side_ring_4_2",
     "//r7081_hqe_ANNIE 3",
     {382, 393, 404, 405}},

    {"PMTINFO_config14_scan_side_ring_5",
     "//r5912_hqe_ANNIE 2",
     {372, 373, 375, 380, 383, 384, 386, 389,
      397, 398, 400, 403, 410, 411, 413, 415}},

    {"PMTINFO_config14_scan_side_ring_6",
     "//r7081_ANNIE 0",
     {416, 422, 423, 426, 430, 437, 438, 442,
      443, 446, 448, 455, 456, 459, 460, 461}},

    {"PMTINFO_config14_scan_topgrid",
     "//d784kflb 1",
     {352, 353, 354, 355, 356, 357, 358, 359, 360, 361,
      362, 363, 364, 365, 366, 367, 368, 369, 370, 371}},

    {"PMTINFO_config14_scan_topgrid_SANDI",
     "//d784kflb 1",
     {352, 353, 354, 355, 356, 357, 358, 359,
      360, 361, 362, 363, 365, 366, 367, 369}},
};
static const size_t kNBlocks = sizeof(kBlocks) / sizeof(kBlocks[0]);

// helpers -----------------------------------------------------------
static std::string trim(const std::string& s) {
  const char* ws = " \t\r\n";
  const size_t a = s.find_first_not_of(ws);
  if (a == std::string::npos) return "";
  const size_t b = s.find_last_not_of(ws);
  return s.substr(a, b - a + 1);
}

// Format a double the way RATDB likes it.  A literal without a decimal point
// would be read back as an integer array which then is nor-readable for ratpac
static std::string fmt(double v, int prec = PRECISION) {
  if (v == 0.0) return "0.0";
  char buf[64];
  std::snprintf(buf, sizeof(buf), "%.*g", prec, v);
  std::string s(buf);
  const bool needs_dot = s.find('.') == std::string::npos &&
                         s.find('e') == std::string::npos &&
                         s.find('E') == std::string::npos &&
                         s.find_first_of("ni") == std::string::npos;  // nan/inf
  if (needs_dot) s += ".0";
  return s;
}

// Unit scaling only.  The data -> RATDB axis swap lives in makePos().
static void transform(PMT& p) {
  p.x *= POS_SCALE;
  p.y *= POS_SCALE;
  p.z *= POS_SCALE;
}

// Data frame -> RATDB frame.  Data y is vertical, RATDB z is vertical.
static void makePos(const PMT& p, double& X, double& Y, double& Z) {
  X = p.x - DATA_AXIS_X + TANK_AXIS_X;
  Y = p.z - DATA_AXIS_Z + TANK_AXIS_Y;
  Z = p.y + VERT_OFFSET;
}

// Exact direction from the position.
static bool makeDir(const PMT& p, double& DX, double& DY, double& DZ) {
  if (p.row == PANEL_BOTTOM) { DX = 0.0; DY = 0.0; DZ =  1.0; return true; }
  if (p.row == PANEL_TOP)    { DX = 0.0; DY = 0.0; DZ = -1.0; return true; }

  double X, Y, Z;
  makePos(p, X, Y, Z);

  const double vx = TANK_AXIS_X - X;
  const double vy = TANK_AXIS_Y - Y;
  const double vz = 0.0;

  const double n = std::sqrt(vx * vx + vy * vy + vz * vz);
  if (n < 1e-9) { DX = 0.0; DY = 0.0; DZ = 0.0; return false; }

  double phi_temp = (45*(p.row-1.0)-67.5)/180.0*3.1415927;

  DX = std::cos(phi_temp);
  DY = std::sin(phi_temp);
  DZ = 0.0;
  return true;
}

// Trailing integer of a comment like "//r7081_ANNIE 0" -> 0 (-1 if absent).
static int commentType(const std::string& comment) {
  const size_t sp = comment.find_last_of(" \t");
  if (sp == std::string::npos) return -1;
  const std::string tail = trim(comment.substr(sp + 1));
  if (tail.empty()) return -1;
  for (size_t i = 0; i < tail.size(); ++i)
    if (!std::isdigit(static_cast<unsigned char>(tail[i]))) return -1;
  return std::atoi(tail.c_str());
}

// table parsing ------------------------------------------------------------ 
static std::map<int, PMT> readTable(const std::string& path) {
  std::ifstream in(path.c_str());
  if (!in) {
    std::cerr << "ERROR: cannot open PMT table '" << path << "'\n";
    std::exit(1);
  }

  std::map<int, PMT> pmts;
  std::string line;
  int nline = 0;
  while (std::getline(in, line)) {
    ++nline;
    const std::string s = trim(line);
    if (s.empty() || s[0] == '#') continue;

    std::istringstream is(s);
    PMT p;
    if (!(is >> p.id >> p.row >> p.x >> p.y >> p.z >> p.dx >> p.dy >> p.dz >>
          p.type)) {
      std::cerr << "WARNING: skipping problem line " << nline << ": " << s
                << "\n";
      continue;
    }
    if (p.row < 0 || p.row > 9)
      std::cerr << "WARNING: PMT " << p.id << " has unexpected panel number "
                << p.row << " (line " << nline << ")\n";
    transform(p);
    if (pmts.count(p.id))
      std::cerr << "WARNING: duplicate PMT ID " << p.id << " (line " << nline
                << "), overwriting\n";
    pmts[p.id] = p;
  }
  return pmts;
}

// ----------------------------------------------------------------- output
static void writeArray(std::ostream& os, const std::string& key,
                       const std::vector<std::string>& v) {
  os << key << ": [";
  for (size_t i = 0; i < v.size(); ++i) os << v[i] << ", ";
  os << "],\n";
}

int main(int argc, char** argv) {
  if (argc < 2 || argc > 3) {
    std::cerr << "usage: " << argv[0] << " <pmt_table.txt> [output.ratdb]\n";
    return 1;
  }

  const std::map<int, PMT> pmts = readTable(argv[1]);

  std::ofstream fout;
  if (argc == 3) {
    fout.open(argv[2]);
    if (!fout) {
      std::cerr << "ERROR: cannot write '" << argv[2] << "'\n";
      return 1;
    }
  }
  std::ostream& out = (argc == 3) ? static_cast<std::ostream&>(fout) : std::cout;

  std::cerr << "read " << pmts.size() << " PMT(s), writing " << kNBlocks << " block(s)\n";
  std::cerr << "directions recomputed from positions about the axis (x, y) = ("
            << TANK_AXIS_X << ", " << TANK_AXIS_Y << ") mm\n";

  std::set<int> used;
  double worstTilt = 0.0;   // largest angle between table dir and computed dir
  int    worstID   = -1;

  // 0: R7081, 1: d784kflb, 2: r5912, 3:R7081
  // Offset between the PMT glass front and the MC PMT center at glass buld equator.
  const double kPMTOffset[4] = {96.7, 106.37, 75.0, 96.7}; 

  for (size_t ib = 0; ib < kNBlocks; ++ib) {
    const Block& blk = kBlocks[ib];

    std::vector<int>         ch;
    std::vector<std::string> X, Y, Z, DX, DY, DZ;
    std::set<int>            types;

    for (size_t i = 0; i < blk.channels.size(); ++i) {
      const int c = blk.channels[i];
      std::map<int, PMT>::const_iterator it = pmts.find(c);
      if (it == pmts.end()) {
        std::cerr << "WARNING: channel " << c << " of block '" << blk.name
                  << "' not found in the PMT table, dropped\n";
        continue;
      }
      const PMT& p = it->second;

      double px, py, pz, dx, dy, dz;
      makePos(p, px, py, pz);
      if (!makeDir(p, dx, dy, dz)) {
        std::cerr << "WARNING: PMT " << c
                  << " sits on the tank axis, direction undefined, dropped\n";
        continue;
      }

      // Correct the PMT positon: Glass position becomes center of PMT position
      px -= kPMTOffset[p.type]*dx;
      py -= kPMTOffset[p.type]*dy;
      pz -= kPMTOffset[p.type]*dz;

      // How far the file panel normal was from the exact radial one.
      {
        const double n = std::sqrt(p.dx * p.dx + p.dy * p.dy + p.dz * p.dz);
        if (n > 0.0) {
          // This table is in the data frame.
          const double ox = p.dx / n, oy = p.dz / n, oz = p.dy / n;
          double d = ox * dx + oy * dy + oz * dz;
          if (d >  1.0) d =  1.0;
          if (d < -1.0) d = -1.0;
          const double ang = std::acos(d) * 180.0 / M_PI;
          if (ang > worstTilt) { worstTilt = ang; worstID = c; }
        }
      }

      ch.push_back(c);
      X.push_back(fmt(px));
      Y.push_back(fmt(py));
      Z.push_back(fmt(pz));

      DX.push_back(fmt(dx, DIR_PREC));
      DY.push_back(fmt(dy, DIR_PREC));
      DZ.push_back(fmt(dz, DIR_PREC));
      types.insert(p.type);
      used.insert(c);
    }

    if (ch.empty()) {
      std::cerr << "WARNING: block '" << blk.name << "' is empty, skipped\n";
      continue;
    }

    // sanity checks on the PMT type -------------------------------------
    if (types.size() > 1)
      std::cerr << "WARNING: block '" << blk.name
                << "' mixes several PMT types\n";
    const int ctype = commentType(blk.comment);
    if (ctype >= 0 && types.size() == 1 && *types.begin() != ctype)
      std::cerr << "WARNING: block '" << blk.name << "' has PMT type "
                << *types.begin() << " but its comment says " << ctype << "\n";

    // write the block ---------------------------------------------------
    out << "{\n";
    out << "name: \"" << blk.name << "\",\n";
    if (!blk.comment.empty()) out << blk.comment << "\n";
    out << "valid_begin: [0, 0],\n";
    out << "valid_end: [0, 0],\n\n";

    writeArray(out, "x", X);
    writeArray(out, "y", Y);
    writeArray(out, "z", Z);
    out << "\n";

    writeArray(out, "dir_x", DX);
    writeArray(out, "dir_y", DY);
    writeArray(out, "dir_z", DZ);
    out << "\n";

    out << "channel_number: [";
    for (size_t i = 0; i < ch.size(); ++i) out << ch[i] << ", ";
    out << "],\n";

    out << "\n}\n\n";
  }

  if (worstID >= 0)
    std::cerr << "largest change wrt the table's dir columns: " << worstTilt
              << " deg (PMT " << worstID << ")\n";

  // --- report PMTs that never made it into a block --------------------------
  for (std::map<int, PMT>::const_iterator it = pmts.begin(); it != pmts.end();
       ++it)
    if (!used.count(it->first))
      std::cerr << "WARNING: PMT ID " << it->first
                << " is in the table but in no block\n";

  return 0;
}