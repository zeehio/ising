#!/bin/bash

outdir="dat/A5"
m=400
PG=

gcc -O2 -o binning ./binning.c || exit 1
gcc -O2 $PG -o ising ./ising.c -lm || exit 1
gcc -O2 -o jackknife ./jackknife.c -lm|| exit 1

mkdir -p $outdir

if [  -f "$outdir/energy.dat" ]; then
   rm "$outdir/energy.dat"
fi

if [  -f "$outdir/magnet.dat" ]; then
   rm "$outdir/magnet.dat"
fi

station=20001
# for each temperature demanded:
for T in "2.0" "2.5" "3.0"; do
    N=10000.0
   # Run simulation:
   ./ising -d 2 -L 100 -T $T --nmeas 1 --nmcs 120000  --ieq 0 -s 107 > "$outdir/${T}_raw.dat"
   # Get the transitory:
   head -n $station < "$outdir/${T}_raw.dat" > "$outdir/${T}_trans.dat"
   # Plot the transitory:
   gnuplot -e "N=$N;input='$outdir/${T}_trans.dat';outE='$outdir/${T}_trans_E.eps';outM='$outdir/${T}_trans_M.eps'" PartA5plot1.gnu 
   # The stationary part:
   tail -n +$station < "$outdir/${T}_raw.dat" > "$outdir/${T}_sta.dat"
   # Jackknife information (using the stationary part)
   echo "T = $T" >> "$outdir/energy.dat"
   echo "T = $T" >> "$outdir/magnet.dat"
   ./mycut -f 1 < "$outdir/${T}_sta.dat" | ./binning -m $m | ./jackknife | sed "s/X/E/g" >> "$outdir/energy.dat"
   ./mycut -f 2 < "$outdir/${T}_sta.dat" | ./binning -m $m | ./jackknife | sed "s/X/M/g" >> "$outdir/magnet.dat"
   echo "" >> "$outdir/energy.dat"
   echo "" >> "$outdir/magnet.dat"
done

gnuplot -e "N=$N;input1='$outdir/2.0_trans.dat';input2='$outdir/2.5_trans.dat';input3='$outdir/3.0_trans.dat';outE='$outdir/all_trans_E.eps';outM='$outdir/all_trans_M.eps'" PartA5plot2.gnu 

