#!/bin/bash
# Johann Martyn

sL="$1" # Get the start line number from the first argument
eL="$2" # Get the end line number from the second argument
selection="$3" #0: uniform position distribution, 1: michel distribution from genie beam-dirt muons

# Error handling: Check if both arguments are provided
if [ -z "$sL" ] || [ -z "$eL" ] || [ -z "$selection" ]; then
    echo "Usage: ./make_throughgoing_muon_macfile.sh.sh <start_line_number> <end_line_number> <infile_selection>"
    exit 1  # Exit with an error code
fi

infile="ANNIE_RATPAC2/utilities/michel_uniform.txt"
if [ $selection -eq 1 ]; then
	infile="ANNIE_RATPAC2/utilities/michel_genie_tracks.txt" 
fi
outfile="ANNIE_RATPAC2/macros/ANNIE_phase2_michels_from_file.mac"

echo "/rat/db/set DETECTOR experiment \"ANNIE\" " > "$outfile"
echo "/rat/db/set DETECTOR geo_file \"ANNIE/ANNIE_run2_config13_scan.geo\" " >> "$outfile"
echo "/run/initialize" >> "$outfile"
echo "/rat/proc splitevdaq" >> "$outfile"
echo "/rat/proclast outntuple" >> "$outfile"
echo "/rat/procset file \"ANNIE_michel_line"$sL"to"$eL".ntuple.root\" " >> "$outfile"
echo "/generator/add combo spectrum:point" >> "$outfile"
echo "/generator/vtx/set e- michel" >> "$outfile"

for (( i=sL; i<=eL; i++ )); do
    line=$(sed -n "${i}p" "$infile")   # Extract the line

    # Check if the line exists
    if [ -z "$line" ]; then
        echo "Line $i not found in $infile"
        exit 2  # Exit with an error code
    fi

    read -r posX posY posZ <<< "$line"
    
	echo "/generator/pos/set $posX $posY $posZ" >> "$outfile"
	echo "/run/beamOn 1" >> "$outfile"
    
done
