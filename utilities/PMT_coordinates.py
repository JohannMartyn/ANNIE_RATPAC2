# This script gives you positions of the PMT on the top, bottom and barrel
# You still have to remove the empty slots (4) on the bottom grid and only keep 8 or 4 PMTs for the mixed rows
# Vincent Fischer, 2020

import math
import numpy as np

### Some variables ##
radius_octogon = 1200.0 #octogon smaller radius in mm
PMT_side_distance = 300.0 #distance between two PMT on the same row of one octet in mm
PMT_offset_vert = 50.0 #vertical offset between 8 and 10" PMTs on the same row (10" are lower)
PMT_offset_hori = 100.0 #horizontal offset between 8 and 10" PMTs on the same row (10" go deeper)

type_r7081_bot = 0
type_etel_top = 1
type_r7081 = 2
type_r5912_hqe = 3
type_r7081_hqe = 4
type_lappd = 5

z_rows = [694.6, 1128.1, 1561.5, 1995.0, 2428.4, 2861.9] #z values of PMT rows in global PMT coordinates

angle_octet = 2.*math.pi/8. #angle between octets
angle_dual_row = 2.*math.atan(PMT_side_distance/(2.*radius_octogon)) # angle between PMT on same row and same octet

ROTATION_ANGLE_TOP_PMTs = 0.0
ROTATION_ANGLE_BOT_PMTs = -math.pi/4.
ROTATION_ANGLE_BARREL_PMTs = -angle_dual_row/2. - angle_octet/2.

### Rotation functions ##
def rotate_point_x(x, y, angle):
  x_rot = x*math.cos(angle) - y*math.sin(angle)
  
  return x_rot
  
def rotate_point_y(x, y, angle):
  y_rot = x*math.sin(angle) + y*math.cos(angle)  
  
  return y_rot
  
### Initialisation ##  
x_pos_bot = []
x_pos_bot_rot = []
y_pos_bot = [] 
y_pos_bot_rot = []
x_pos_top = []
x_pos_top_rot = []
y_pos_top = []
y_pos_top_rot = []
z_pos_top = []

x_pos_8inch = []
x_pos_8inch_rot = []
y_pos_8inch = []
y_pos_8inch_rot = []
x_pos_10inch = []
x_pos_10inch_rot = []
y_pos_10inch = []
y_pos_10inch_rot = []
x_pos_lappd = []
x_pos_lappd_rot = []
y_pos_lappd = []
y_pos_lappd_rot = []
dir_x = []
dir_y = []
dir_z = []
dir_x_bot = []
dir_y_bot = []
dir_z_bot = []
dir_x_top = []
dir_y_top = []
dir_z_top = []
dir_x_lappd = []
dir_y_lappd = []
dir_z_lappd = []

for i in range(0,20):
	dir_x_bot.append(0.0)
	dir_y_bot.append(0.0)
	dir_z_bot.append(1.0)
	dir_x_top.append(0.0)
	dir_y_top.append(0.0)
	dir_z_top.append(-1.0)

### Bottom PMTs ##
Bot_PMT_spacing_bulbside = 457.0 #in mm 
Bot_PMT_spacing_wingside = 280.0 #in mm

for i in range(0,4):
	x_pos_bot.append(-Bot_PMT_spacing_bulbside/2. - Bot_PMT_spacing_bulbside)
for i in range(0,6):
	x_pos_bot.append(-Bot_PMT_spacing_bulbside/2.)
for i in range(0,6):
	x_pos_bot.append(Bot_PMT_spacing_bulbside/2.)
for i in range(0,4):
	x_pos_bot.append(Bot_PMT_spacing_bulbside/2. + Bot_PMT_spacing_bulbside)	

y_pos_bot.append(Bot_PMT_spacing_wingside/2. + Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2. + 2*Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2. + Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - Bot_PMT_spacing_wingside)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - 2*Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2. + 2*Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2. + Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - Bot_PMT_spacing_wingside)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - 2*Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2. + Bot_PMT_spacing_wingside)
y_pos_bot.append(Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2.)
y_pos_bot.append(-Bot_PMT_spacing_wingside/2. - Bot_PMT_spacing_wingside)

for i in range(0,len(x_pos_bot)):
	x_pos_bot_rot.append(round(rotate_point_x(x_pos_bot[i],y_pos_bot[i],ROTATION_ANGLE_BOT_PMTs),3))
	y_pos_bot_rot.append(round(rotate_point_y(x_pos_bot[i],y_pos_bot[i],ROTATION_ANGLE_BOT_PMTs),3))

### Top PMTs ##
Top_PMT_hatch_radius = 330.0 #in mm
Top_PMT_radius = 813.0 
Top_PMT_pair_separation = 292.0

x_pos_top.append(Top_PMT_hatch_radius)
y_pos_top.append(0.0)
z_pos_top.append(3305.2000 + 57.0)
# Rotate 3 more times to get 4 positions for the hatch
for i in range(1,4):
	x_pos_top.append(round(rotate_point_x(x_pos_top[i-1],y_pos_top[i-1],2.*math.pi/4.),3))
	y_pos_top.append(round(rotate_point_y(x_pos_top[i-1],y_pos_top[i-1],2.*math.pi/4.),3))
	z_pos_top.append(round(3305.2000 + 57.0, 3)) #hatch PMTs are a little higher
	
# Now the PMT on the side (not on the hatch)
x_pos_top.append(round(Top_PMT_radius, 3))
y_pos_top.append(round(0.0 + Top_PMT_pair_separation/2., 3))
z_pos_top.append(round(3305.2000, 3))
x_pos_top.append(round(Top_PMT_radius, 3))
y_pos_top.append(round(0.0 - Top_PMT_pair_separation/2., 3))
z_pos_top.append(round(3305.2000, 3))
# Rotate 7 more times to get 8 positions (by pair)
for i in range(4,17,2):
	x_pos_top.append(round(rotate_point_x(x_pos_top[i],y_pos_top[i],angle_octet),3))
	y_pos_top.append(round(rotate_point_y(x_pos_top[i],y_pos_top[i],angle_octet),3))
	z_pos_top.append(round(3305.2000, 3))
	x_pos_top.append(round(rotate_point_x(x_pos_top[i+1],y_pos_top[i+1],angle_octet),3))
	y_pos_top.append(round(rotate_point_y(x_pos_top[i+1],y_pos_top[i+1],angle_octet),3))
	z_pos_top.append(round(3305.2000, 3))

# Rotate position if needed	
for i in range(0,len(x_pos_top)):
	x_pos_top_rot.append(round(rotate_point_x(x_pos_top[i],y_pos_top[i],ROTATION_ANGLE_TOP_PMTs),3))
	y_pos_top_rot.append(round(rotate_point_y(x_pos_top[i],y_pos_top[i],ROTATION_ANGLE_TOP_PMTs),3))

### Barrel PMTs ##

# First point of the array
x_pos_8inch.append(radius_octogon)
y_pos_8inch.append(0.0)
x_pos_10inch.append(radius_octogon - PMT_offset_hori)
y_pos_10inch.append(0.0)
x_pos_lappd.append(round(rotate_point_x(radius_octogon - 50.0, 0.0, 0.65*angle_octet),3))
y_pos_lappd.append(round(rotate_point_y(radius_octogon - 50.0, 0.0, 0.65*angle_octet),3))

# Rotate 7 more times to get 8 positions
for i in range(1,8):
  x_pos_8inch.append(round(rotate_point_x(x_pos_8inch[i-1],y_pos_8inch[i-1],angle_octet),3))
  y_pos_8inch.append(round(rotate_point_y(x_pos_8inch[i-1],y_pos_8inch[i-1],angle_octet),3))
  x_pos_10inch.append(round(rotate_point_x(x_pos_10inch[i-1],y_pos_10inch[i-1],angle_octet),3))
  y_pos_10inch.append(round(rotate_point_y(x_pos_10inch[i-1],y_pos_10inch[i-1],angle_octet),3))
  x_pos_lappd.append(round(rotate_point_x(x_pos_lappd[i-1],y_pos_lappd[i-1],angle_octet),3))
  y_pos_lappd.append(round(rotate_point_y(x_pos_lappd[i-1],y_pos_lappd[i-1],angle_octet),3))

# Rotate 8 more times for the 16 positions
for i in range(0,8):
  x_pos_8inch.append(round(rotate_point_x(x_pos_8inch[i],y_pos_8inch[i],angle_dual_row),3))
  y_pos_8inch.append(round(rotate_point_y(x_pos_8inch[i],y_pos_8inch[i],angle_dual_row),3))
  x_pos_10inch.append(round(rotate_point_x(x_pos_10inch[i],y_pos_10inch[i],angle_dual_row),3))
  y_pos_10inch.append(round(rotate_point_y(x_pos_10inch[i],y_pos_10inch[i],angle_dual_row),3))
  
# Rotate position if needed	
for i in range(0,len(x_pos_8inch)):
	x_pos_8inch_rot.append(round(rotate_point_x(x_pos_8inch[i],y_pos_8inch[i],ROTATION_ANGLE_BARREL_PMTs),3))
	y_pos_8inch_rot.append(round(rotate_point_y(x_pos_8inch[i],y_pos_8inch[i],ROTATION_ANGLE_BARREL_PMTs),3))  
	
# Rotate position if needed	
for i in range(0,len(x_pos_10inch)):
	x_pos_10inch_rot.append(round(rotate_point_x(x_pos_10inch[i],y_pos_10inch[i],ROTATION_ANGLE_BARREL_PMTs),3))
	y_pos_10inch_rot.append(round(rotate_point_y(x_pos_10inch[i],y_pos_10inch[i],ROTATION_ANGLE_BARREL_PMTs),3))
	
# Rotate position if needed	
for i in range(0,len(x_pos_lappd)):
	x_pos_lappd_rot.append(round(rotate_point_x(x_pos_lappd[i],y_pos_lappd[i],ROTATION_ANGLE_BARREL_PMTs),3))
	y_pos_lappd_rot.append(round(rotate_point_y(x_pos_lappd[i],y_pos_lappd[i],ROTATION_ANGLE_BARREL_PMTs),3))	

# Get 16 directions
for i in range(0,16):
  dir_x.append(round(-2.*x_pos_8inch_rot[i],3))
  dir_y.append(round(-2.*y_pos_8inch_rot[i],3))
  dir_z.append(0.0)

### Now printing row by row ##

print "Top ( 11 inch)"
z_pos = []
type = []

for i in range(0,20):
  type.append(type_etel_top)
  
print "x:", x_pos_top_rot
print "y:", y_pos_top_rot
print "z:", z_pos_top
print ""
print "dir_x:", dir_x_top
print "dir_y:", dir_y_top
print "dir_z:", dir_z_top
print "type:", type

print ""
print ""
print "Bottom ( 10 inch)"
z_pos = []
type = []

for i in range(0,20):
  z_pos.append(215.7)
  type.append(type_r7081_bot)
  
print "x:", x_pos_bot_rot
print "y:", y_pos_bot_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x_bot
print "dir_y:", dir_y_bot
print "dir_z:", dir_z_bot
print "type:", type

print ""
print ""
print "Side row 1 (lower, 10 inch, empty are 16-14-12-10)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[0] - PMT_offset_vert,3))
  type.append(type_r7081)
  
print "x:", x_pos_10inch_rot
print "y:", y_pos_10inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 2 (8 inch are 1-8)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[1],3))
  type.append(type_r5912_hqe)
  
print "x:", x_pos_8inch_rot
print "y:", y_pos_8inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 2 (10 inch are 9-16)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[1] - PMT_offset_vert,3))
  type.append(type_r7081)
  
print "x:", x_pos_10inch_rot
print "y:", y_pos_10inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 3 (8 inch)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[2],3))
  type.append(type_r5912_hqe)
  
print "x:", x_pos_8inch_rot
print "y:", y_pos_8inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 4 (10 inch)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[3] - PMT_offset_vert,3))
  type.append(type_r7081)
  
print "x:", x_pos_10inch_rot
print "y:", y_pos_10inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 4 (10 inch HQE are 10-12-14-16)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[3] - PMT_offset_vert,3))
  type.append(type_r7081_hqe)
  
print "x:", x_pos_10inch_rot
print "y:", y_pos_10inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 5 (8 inch)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[4],3))
  type.append(type_r5912_hqe)
  
print "x:", x_pos_8inch_rot
print "y:", y_pos_8inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print ""
print ""
print "Side row 6 (higher, 10 inch)"
z_pos = []
type = []

for i in range(0,16):
  z_pos.append(round(z_rows[5] - PMT_offset_vert,3))
  type.append(type_r7081)
  
print "x:", x_pos_10inch_rot
print "y:", y_pos_10inch_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x
print "dir_y:", dir_y
print "dir_z:", dir_z
print "type:", type

print "\n\nLAPPD positions\n\n"
print "Side 1"
z_pos = []
type = []
dir_x_lappd = []
dir_y_lappd = []
dir_z_lappd = []
for i in range(0,8):
  z_pos.append(round(z_rows[0],3))
  type.append(type_lappd)
  #dir_x_lappd.append(round(math.cos(math.pi-math.atan2(y_pos_lappd[i],x_pos_lappd[i])),3))
  #dir_y_lappd.append(round(math.cos(math.pi/2+math.atan2(y_pos_lappd[i],x_pos_lappd[i])),3))
  dir_x_lappd.append(round(-x_pos_lappd_rot[i],3))
  dir_y_lappd.append(round(-y_pos_lappd_rot[i],3))
  dir_z_lappd.append(0.0)
  
print "x:", x_pos_lappd_rot
print "y:", y_pos_lappd_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x_lappd
print "dir_y:", dir_y_lappd
print "dir_z:", dir_z_lappd
print "type:", type



print "\n\nSide 3"
z_pos = []
type = []
dir_x_lappd = []
dir_y_lappd = []
dir_z_lappd = []
for i in range(0,8):
  z_pos.append(round(z_rows[2],3))
  type.append(type_lappd)
  dir_x_lappd.append(round(-2.*x_pos_lappd_rot[i],3))
  dir_y_lappd.append(round(-2.*y_pos_lappd_rot[i],3))
  dir_z_lappd.append(0.0)
  
print "x:", x_pos_lappd_rot
print "y:", y_pos_lappd_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x_lappd
print "dir_y:", dir_y_lappd
print "dir_z:", dir_z_lappd
print "type:", type

print "\n\nSide 5"
z_pos = []
type = []
dir_x_lappd = []
dir_y_lappd = []
dir_z_lappd = []
for i in range(0,8):
  z_pos.append(round(z_rows[4],3))
  type.append(type_lappd)
  dir_x_lappd.append(round(-2.*x_pos_lappd_rot[i],3))
  dir_y_lappd.append(round(-2.*y_pos_lappd_rot[i],3))
  dir_z_lappd.append(0.0)
  
print "x:", x_pos_lappd_rot
print "y:", y_pos_lappd_rot
print "z:", z_pos
print ""
print "dir_x:", dir_x_lappd
print "dir_y:", dir_y_lappd
print "dir_z:", dir_z_lappd
print "type:", type
