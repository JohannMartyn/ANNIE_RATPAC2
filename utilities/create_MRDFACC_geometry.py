import math

### Some variables ##
# Veto wall (concrete) center in hall coordinates is (0; -2235; -2845)
FACC_wall_center = [0, -2235, -2845]
nb_FACC_row = 13
nb_FACC_layer = 2
offset_FACC_layers = 10. #offset between FACC layers so they overlap a little, in mm

offset_MRD = 2.*25.4 + 200. + 2.*1524. + 200 # distance betwwen veto concrete wall and first layer of MRD (a steel plate) (2 FACC layers + gap + tank + gap)
offset_MRD_layers = 50.8 # air gap between scintillator and next steel plate in z, in mm

nb_MRD_hori_layer = 6
nb_MRD_hori_row = 13
nb_MRD_vert_layer = 5
nb_MRD_vert_row = [15, 17, 13, 15, 15] #in beam direction

FACC_paddle_size = [1610.0, 155.0, 12.7] # half-length in x,y,z
MRD_steel_plate_size = [1525.0, 1370.0, 25.4]
MRD_hori_paddle_size = [775.0, 100.0, 3.0]
MRD_vert_paddle_size_1 = [100.0, 690.0, 4.0] # old paddles
MRD_vert_paddle_size_2 = [75.0, 690.0, 7.0] # new narrower paddles


# function to write a paddle geometry
def print_scint_paddle_geometry(volume_name, pos_x, pos_y, pos_z, size_x, size_y, size_z):
  print "{"
  print "name: \"GEO\","
  print "index: \"%s\"," % volume_name
  print "valid_begin: [0, 0],"
  print "valid_end: [0, 0],"
  print "mother: \"hall\","
  print "type: \"box\","
  print "size: [%s, %s, %s], // mm, half-length" % (round(size_x,1), round(size_y,1), round(size_z,1))
  print "position: [%s, %s, %s]," % (round(pos_x,1), round(pos_y,1), round(pos_z,1))
  print "color: [0.2, 0.8, 0.2, 0.5],"
  print "material: \"polystyrene\","
  print "}"
  
# function to write a steel plate geometry
def print_steel_plate_geometry(volume_name, pos_x, pos_y, pos_z):
  print "{"
  print "name: \"GEO\","
  print "index: \"%s\"," % volume_name
  print "valid_begin: [0, 0],"
  print "valid_end: [0, 0],"
  print "mother: \"hall\","
  print "type: \"box\","
  print "size: [1525.0, 1370.0, 25.4], // mm, half-length" 
  print "position: [%s, %s, %s]," % (round(pos_x,1), round(pos_y,1), round(pos_z,1))
  print "color: [0.8, 0.4, 0.0, 0.5],"
  print "material: \"steel\","
  print "}"  
  
  
  
## Looping on FACC positions
# First layer (from beam) is z=0, second is z=1
# Bottom paddle is y=0

print "////// FACC paddles //// \n\n"

for i in range(0,13):
  for j in range(0,2):
    paddle_name = "FACC_paddle_z" + `j` + "_y" + `i`
    print_scint_paddle_geometry(paddle_name, FACC_wall_center[0], FACC_wall_center[1] - (6-i)*2.*FACC_paddle_size[1] + j*offset_FACC_layers, FACC_wall_center[2] + (2*j+1)*FACC_paddle_size[2], FACC_paddle_size[0], FACC_paddle_size[1], FACC_paddle_size[2])
    
print "\n\n"

## Now looping  on MRD paddles and steel plates  
  
print "////// MRD paddles and steel plates //// \n\n"

# Steel plate 1
steel_plate_1_center = [FACC_wall_center[0], FACC_wall_center[1], FACC_wall_center[2] + offset_MRD + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_1", steel_plate_1_center[0], steel_plate_1_center[1], steel_plate_1_center[2])

# Horizontal layer 2
hori_layer_2_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_1_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z2_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_2_center[0]-MRD_hori_paddle_size[0], hori_layer_2_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_2_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z2_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_2_center[0]+MRD_hori_paddle_size[0], hori_layer_2_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_2_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])


# Steel plate 2
steel_plate_2_center = [FACC_wall_center[0], FACC_wall_center[1], hori_layer_2_center[2] + MRD_hori_paddle_size[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_2", steel_plate_2_center[0], steel_plate_2_center[1], steel_plate_2_center[2])

# Vertical layer 3
vert_layer_3_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_2_center[2] + MRD_steel_plate_size[2] + MRD_vert_paddle_size_2[2]]
for i in range(0,nb_MRD_vert_row[0]):
    paddle_name = "MRD_paddle_z3_x"+ `i` + "_y0" 
    print_scint_paddle_geometry(paddle_name, vert_layer_3_center[0] - (7-i)*2.*MRD_vert_paddle_size_2[0], vert_layer_3_center[1]-MRD_vert_paddle_size_2[1], vert_layer_3_center[2], MRD_vert_paddle_size_2[0], MRD_vert_paddle_size_2[1], MRD_vert_paddle_size_2[2])
    paddle_name = "MRD_paddle_z3_x"+ `i` + "_y1"
    print_scint_paddle_geometry(paddle_name, vert_layer_3_center[0] - (7-i)*2.*MRD_vert_paddle_size_2[0], vert_layer_3_center[1]+MRD_vert_paddle_size_2[1], vert_layer_3_center[2], MRD_vert_paddle_size_2[0], MRD_vert_paddle_size_2[1], MRD_vert_paddle_size_2[2])

#####################################################################################

# Steel plate 3
steel_plate_3_center = [FACC_wall_center[0], FACC_wall_center[1], vert_layer_3_center[2] + MRD_vert_paddle_size_2[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_3", steel_plate_3_center[0], steel_plate_3_center[1], steel_plate_3_center[2])

# Horizontal layer 4
hori_layer_4_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_3_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z4_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_4_center[0]-MRD_hori_paddle_size[0], hori_layer_4_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_4_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z4_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_4_center[0]+MRD_hori_paddle_size[0], hori_layer_4_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_4_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])


# Steel plate 4
steel_plate_4_center = [FACC_wall_center[0], FACC_wall_center[1], hori_layer_4_center[2] + MRD_hori_paddle_size[2] + offset_MRD_layers + MRD_vert_paddle_size_2[2]]
print_steel_plate_geometry("MRD_steel_plate_4", steel_plate_4_center[0], steel_plate_4_center[1], steel_plate_4_center[2])

# Vertical layer 5
vert_layer_5_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_4_center[2] + MRD_steel_plate_size[2] + MRD_vert_paddle_size_2[2]]
for i in range(0,nb_MRD_vert_row[1]):
    paddle_name = "MRD_paddle_z5_x"+ `i` + "_y0" 
    print_scint_paddle_geometry(paddle_name, vert_layer_5_center[0] - (8-i)*2.*MRD_vert_paddle_size_2[0], vert_layer_5_center[1]-MRD_vert_paddle_size_2[1], vert_layer_5_center[2], MRD_vert_paddle_size_2[0], MRD_vert_paddle_size_2[1], MRD_vert_paddle_size_2[2])
    paddle_name = "MRD_paddle_z5_x"+ `i` + "_y1"
    print_scint_paddle_geometry(paddle_name, vert_layer_5_center[0] - (8-i)*2.*MRD_vert_paddle_size_2[0], vert_layer_5_center[1]+MRD_vert_paddle_size_2[1], vert_layer_5_center[2], MRD_vert_paddle_size_2[0], MRD_vert_paddle_size_2[1], MRD_vert_paddle_size_2[2])


#####################################################################################

# Steel plate 5
steel_plate_5_center = [FACC_wall_center[0], FACC_wall_center[1], vert_layer_5_center[2] + MRD_vert_paddle_size_2[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_5", steel_plate_5_center[0], steel_plate_5_center[1], steel_plate_5_center[2])

# Horizontal layer 6
hori_layer_6_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_5_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z6_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_6_center[0]-MRD_hori_paddle_size[0], hori_layer_6_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_6_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z6_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_6_center[0]+MRD_hori_paddle_size[0], hori_layer_6_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_6_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])


# Steel plate 6
steel_plate_6_center = [FACC_wall_center[0], FACC_wall_center[1], hori_layer_6_center[2] + MRD_hori_paddle_size[2] + offset_MRD_layers + MRD_vert_paddle_size_1[2]]
print_steel_plate_geometry("MRD_steel_plate_6", steel_plate_6_center[0], steel_plate_6_center[1], steel_plate_6_center[2])

# Vertical layer 7
vert_layer_7_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_6_center[2] + MRD_steel_plate_size[2] + MRD_vert_paddle_size_1[2]]
for i in range(0,nb_MRD_vert_row[2]):
    paddle_name = "MRD_paddle_z7_x"+ `i` + "_y0" 
    print_scint_paddle_geometry(paddle_name, vert_layer_7_center[0] - (6-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_7_center[1]-MRD_vert_paddle_size_1[1], vert_layer_7_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2])
    paddle_name = "MRD_paddle_z7_x"+ `i` + "_y1"
    print_scint_paddle_geometry(paddle_name, vert_layer_7_center[0] - (6-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_7_center[1]+MRD_vert_paddle_size_1[1], vert_layer_7_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2])
    
    
#####################################################################################

# Steel plate 7
steel_plate_7_center = [FACC_wall_center[0], FACC_wall_center[1], vert_layer_7_center[2] + MRD_vert_paddle_size_1[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_7", steel_plate_7_center[0], steel_plate_7_center[1], steel_plate_7_center[2])

# Horizontal layer 8
hori_layer_8_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_7_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z8_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_8_center[0]-MRD_hori_paddle_size[0], hori_layer_8_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_8_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z8_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_8_center[0]+MRD_hori_paddle_size[0], hori_layer_8_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_8_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])


# Steel plate 8
steel_plate_8_center = [FACC_wall_center[0], FACC_wall_center[1], hori_layer_8_center[2] + MRD_hori_paddle_size[2] + offset_MRD_layers + MRD_vert_paddle_size_1[2]]
print_steel_plate_geometry("MRD_steel_plate_8", steel_plate_8_center[0], steel_plate_8_center[1], steel_plate_8_center[2])

# Vertical layer 9
vert_layer_9_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_8_center[2] + MRD_steel_plate_size[2] + MRD_vert_paddle_size_1[2]]
for i in range(0,nb_MRD_vert_row[3]):
    paddle_name = "MRD_paddle_z9_x"+ `i` + "_y0" 
    print_scint_paddle_geometry(paddle_name, vert_layer_9_center[0] - (7-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_9_center[1]-MRD_vert_paddle_size_1[1], vert_layer_9_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2])
    paddle_name = "MRD_paddle_z9_x"+ `i` + "_y1"
    print_scint_paddle_geometry(paddle_name, vert_layer_9_center[0] - (7-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_9_center[1]+MRD_vert_paddle_size_1[1], vert_layer_9_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2])    


#####################################################################################

# Steel plate 9
steel_plate_9_center = [FACC_wall_center[0], FACC_wall_center[1], vert_layer_9_center[2] + MRD_vert_paddle_size_1[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_9", steel_plate_9_center[0], steel_plate_9_center[1], steel_plate_9_center[2])

# Horizontal layer 10
hori_layer_10_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_9_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z10_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_10_center[0]-MRD_hori_paddle_size[0], hori_layer_10_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_10_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z10_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_10_center[0]+MRD_hori_paddle_size[0], hori_layer_10_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_10_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])


# Steel plate 10
steel_plate_10_center = [FACC_wall_center[0], FACC_wall_center[1], hori_layer_10_center[2] + MRD_hori_paddle_size[2] + offset_MRD_layers + MRD_vert_paddle_size_1[2]]
print_steel_plate_geometry("MRD_steel_plate_10", steel_plate_10_center[0], steel_plate_10_center[1], steel_plate_10_center[2])

# Vertical layer 11
vert_layer_11_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_10_center[2] + MRD_steel_plate_size[2] + MRD_vert_paddle_size_1[2]]
for i in range(0,nb_MRD_vert_row[4]):
    paddle_name = "MRD_paddle_z11_x"+ `i` + "_y0" 
    print_scint_paddle_geometry(paddle_name, vert_layer_11_center[0] - (7-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_11_center[1]-MRD_vert_paddle_size_1[1], vert_layer_11_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2])
    paddle_name = "MRD_paddle_z11_x"+ `i` + "_y1"
    print_scint_paddle_geometry(paddle_name, vert_layer_11_center[0] - (7-i)*2.*MRD_vert_paddle_size_1[0], vert_layer_11_center[1]+MRD_vert_paddle_size_1[1], vert_layer_11_center[2], MRD_vert_paddle_size_1[0], MRD_vert_paddle_size_1[1], MRD_vert_paddle_size_1[2]) 


#####################################################################################

# Steel plate 11
steel_plate_11_center = [FACC_wall_center[0], FACC_wall_center[1], vert_layer_11_center[2] + MRD_vert_paddle_size_1[2] + offset_MRD_layers + MRD_steel_plate_size[2]]
print_steel_plate_geometry("MRD_steel_plate_11", steel_plate_11_center[0], steel_plate_11_center[1], steel_plate_11_center[2])

# Horizontal layer 12
hori_layer_12_center = [FACC_wall_center[0], FACC_wall_center[1], steel_plate_11_center[2] + MRD_steel_plate_size[2] + MRD_hori_paddle_size[2]]
for i in range(0,nb_MRD_hori_row):
    paddle_name = "MRD_paddle_z12_x0_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_12_center[0]-MRD_hori_paddle_size[0], hori_layer_12_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_12_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])
    paddle_name = "MRD_paddle_z12_x1_y" + `i`
    print_scint_paddle_geometry(paddle_name, hori_layer_12_center[0]+MRD_hori_paddle_size[0], hori_layer_12_center[1] - (6-i)*2.*MRD_hori_paddle_size[1], hori_layer_12_center[2], MRD_hori_paddle_size[0], MRD_hori_paddle_size[1], MRD_hori_paddle_size[2])









