#!/bin/bash
# Johann Martyn

sL="$1" # Get the start line number from the first argument
eL="$2" # Get the end line number from the second argument
infile="ANNIE_RATPAC2/utilities/throughgoing_muon_selection.txt"
outfile="ANNIE_RATPAC2/macros/ANNIE_phase2_muons_from_file.mac"

# Error handling: Check if both arguments are provided
if [ -z "$sL" ] || [ -z "$eL" ] ; then
    echo "Usage: ./make_throughgoing_muon_macfile.sh.sh <start_line_number> <end_line_number>"
    exit 1  # Exit with an error code
fi

echo "/rat/db/set DETECTOR experiment \"ANNIE\" " > "$outfile"
echo "/rat/db/set DETECTOR geo_file \"ANNIE/ANNIE_run2_config13_scan.geo\" " >> "$outfile"
echo "/run/initialize" >> "$outfile"
echo "/rat/proc simpledaq" >> "$outfile"
echo "/rat/proclast outntuple" >> "$outfile"
echo "/rat/procset file \"ANNIE_throughmuon_1GeV_0.5xQE_line"$sL"to"$eL".ntuple.root\" " >> "$outfile"
echo "/generator/add combo gun:point" >> "$outfile"

for (( i=sL; i<=eL; i++ )); do
    line=$(sed -n "${i}p" "$infile")   # Extract the line

    # Check if the line exists
    if [ -z "$line" ]; then
        echo "Line $i not found in $infile"
        exit 2  # Exit with an error code
    fi

    read -r posX posY posZ dirX dirY dirZ <<< "$line"
    
	echo "/generator/vtx/set mu- $dirX $dirY $dirZ 1000.0" >> "$outfile"
	echo "/generator/pos/set $posX $posY $posZ" >> "$outfile"
	echo "/run/beamOn 1" >> "$outfile"
    
done


