#!/bin/bash
# Johann Martyn

sL="$1" # Get the start line number from the first argument
eL="$2" # Get the end line number from the second argument
infile="ANNIE_RATPAC2/utilities/genie_protons_PMTEnclosedVol.txt"
outfile="ANNIE_RATPAC2/macros/ANNIE_phase2_protons_from_file.mac"

# Error handling: Check if both arguments are provided
if [ -z "$sL" ] || [ -z "$eL" ] ; then
    echo "Usage: ./make_proton_macfile.sh <start_line_number> <end_line_number>"
    exit 1  # Exit with an error code
fi

echo "/tracking/storeTrajectory 1" > "$outfile"
echo "/tracking/discardParticleTrajectory opticalphoton" >> "$outfile"
echo "/process/had/particle_hp/use_photo_evaporation true" >> "$outfile"
echo "/process/had/particle_hp/do_not_adjust_final_state true" >> "$outfile"
echo "/rat/db/set DETECTOR experiment \"ANNIE\" " >> "$outfile"
echo "/rat/db/set DETECTOR geo_file \"ANNIE/ANNIE_run2_config13_scan.geo\" " >> "$outfile"
echo "/rat/db/load ANNIE/ANNIE_WbLS_Volume_Johann.geo " >> "$outfile"
echo "/run/initialize" >> "$outfile"
echo "/rat/proc splitevdaq" >> "$outfile"
echo "/rat/proclast outanniecluster" >> "$outfile"
echo "/rat/procset file \"BIGSANDI_protons_MichelChargeTunedQE_0.8xR_line"$sL"to"$eL".ntuple.root\" " >> "$outfile"
echo "/generator/add combo gun:point" >> "$outfile"

for (( i=sL; i<=eL; i++ )); do
    line=$(sed -n "${i}p" "$infile")   # Extract the line

    # Check if the line exists
    if [ -z "$line" ]; then
        echo "Line $i not found in $infile"
        exit 2  # Exit with an error code
    fi

    read -r posX posY posZ dirX dirY dirZ mE <<< "$line"
    
	echo "/generator/vtx/set proton $dirX $dirY $dirZ $mE" >> "$outfile"
	echo "/generator/pos/set $posX $posY $posZ" >> "$outfile"
	echo "/run/beamOn 1" >> "$outfile"
    
done


