#!/bin/bash

outdir="dat/A7"
m=400
PG=

gcc -O2 -o binning ./binning.c || exit 1
gcc -O2 $PG -o ising ./ising.c -lm || exit 1
gcc -O2 -o jackknife ./jackknife.c -lm|| exit 1

mkdir -p $outdir
if [ -f "$outdir/energy.dat" ]; then
    rm "$outdir/energy.dat"
fi
if [ -f "$outdir/magnet.dat" ]; then
    rm "$outdir/magnet.dat"
fi

station=20001
# for each temperature
for T in "2.0" "2.5" "3.0";do
	# for each type of dynamics demanded:
	for dyn in "0" "1" "2"; do
		N=10000.0
#		T=2.0
	   # Run simulation:
	   ./ising -d 2 -L 100 -T $T --nmeas 1 --nmcs 100000  --ieq 0 -s 107 --dyn $dyn > "$outdir/${T}_${dyn}_raw.dat"
	   # Get the transitory:
	   head -n $station < "$outdir/${T}_${dyn}_raw.dat" > "$outdir/${T}_${dyn}_trans.dat"
	   # Plot the transitory:
	   gnuplot -e "N=$N;input='$outdir/${T}_${dyn}_trans.dat';outE='$outdir/${T}_${dyn}_trans_E.eps';outM='$outdir/${T}_${dyn}_trans_M.eps'" PartA7plot1.gnu 
	   # The stationary part:
	   tail -n +$station < "$outdir/${T}_${dyn}_raw.dat" > "$outdir/${T}_${dyn}_sta.dat"
	   # Jackknife information (using the stationary part)
	   echo "T = $T" >> "$outdir/energy.dat"
	   echo "T = $T" >> "$outdir/magnet.dat"
	   ./mycut -f 1 < "$outdir/${T}_${dyn}_sta.dat" | ./binning -m $m | ./jackknife >> "$outdir/energy.dat"
	   ./mycut -f 2 < "$outdir/${T}_${dyn}_sta.dat" | ./binning -m $m | ./jackknife >> "$outdir/magnet.dat"
	   echo "" >> "$outdir/energy.dat"
	   echo "" >> "$outdir/magnet.dat"
	done
gnuplot -e "N=$N;input1='$outdir/${T}_0_trans.dat';input2='$outdir/${T}_1_trans.dat';input3='$outdir/${T}_2_trans.dat';outE='$outdir/${T}_all_trans_E.eps';outM='$outdir/${T}_all_trans_M.eps'" PartA7plot2.gnu 

done


