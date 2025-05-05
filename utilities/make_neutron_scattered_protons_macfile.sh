#!/bin/bash
# Johann Martyn

eV="$1" # Get the event number, i.e. the textfile to read in
infile="ANNIE_RATPAC2/utilities/genie_muon_neutrons/output_$eV.txt"
outfile="ANNIE_RATPAC2/macros/ANNIE_phase2_genie_neutrons_from_file.mac"

# Error handling: Check if both arguments are provided
if [ -z "$eV" ]; then
    echo "Usage: ./make_neutron_scattered_protons_macfile.sh <event_number>"
    exit 1  # Exit with an error code
fi

echo "/tracking/storeTrajectory 1" > "$outfile"
echo "/tracking/discardParticleTrajectory opticalphoton" >> "$outfile"
echo "/rat/db/set DETECTOR experiment \"ANNIE\" " >> "$outfile"
echo "/rat/db/set DETECTOR geo_file \"ANNIE/ANNIE_run2_config13_scan.geo\" " >> "$outfile"
#echo "/rat/db/load ANNIE/ANNIE_WbLS_Volume_Johann.geo " >> "$outfile"
echo "/run/initialize" >> "$outfile"
echo "/rat/proc splitevdaq" >> "$outfile"
echo "/rat/proclast outanniecluster" >> "$outfile"
echo "/rat/procset file \"ANNIE_genie_neutrons_untunedQE_0.8xR_event"$eV".ntuple.root\" " >> "$outfile"
echo "/generator/add combo gun:point" >> "$outfile"

isMuon=true

while read -r line; do
	#line=$(sed -n "${i}p" "$infile")   # Extract the line

	# Check if the line exists
	if [ -z "$line" ]; then
		echo "Line $i not found in $infile"
		exit 2  # Exit with an error code
	fi

	read -r posX posY posZ dirX dirY dirZ mE <<< "$line"
	
	case $isMuon in
	  (true)    echo "/generator/vtx/set mu- $dirX $dirY $dirZ $mE" >> "$outfile"
	  			isMuon=false;;
	  (false)   echo "/generator/vtx/set neutron $dirX $dirY $dirZ $mE" >> "$outfile";;
	esac

	echo "/generator/pos/set $posX $posY $posZ" >> "$outfile"
	echo "/run/beamOn 1" >> "$outfile"
done < "$infile"


