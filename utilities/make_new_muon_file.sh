#!/bin/bash
# Johann Martyn

infile="throughgoing_genie_muons_forRAT.txt"
outfile="throughgoing_genie_muons_forRAT_smaller2GeV.txt"

for (( i=30001; i<=40518; i++ )); do
    line=$(sed -n "${i}p" "$infile")   # Extract the line

    # Check if the line exists
    if [ -z "$line" ]; then
        echo "Line $i not found in $infile"
        exit 2  # Exit with an error code
    fi

    read -r posX posY posZ dirX dirY dirZ mE <<< "$line"

	echo "$posX $posY $posZ $dirX $dirY $dirZ $mE" >> "$outfile"
       
	
    
done


