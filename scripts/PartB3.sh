#!/bin/bash

outdir="dat/B3"
data="$outdir/5.dat"
fitxenergies="$outdir/5_energies.dat"
#outfile="$outdir/energies.dat"
#outplot="$outdir/energies.eps"
PG=
#PG=" -pg "

mkdir -p $outdir

gcc -O3 -o autocorr autocorr.c -lm || exit 1
gcc -O3 $PG -o ising ising.c -lm || exit 1

L[0]="20"
L[1]="40"
L[2]="100"
N[0]="400"
N[1]="1600"
N[2]="10000"
rangeE["0"]="3000"
rangeE["3"]="10"
rangeM["0"]="3000"
rangeM["3"]="250"

for j in "0" "3";do
   for i in $(seq 0 2);do
      echo "Computing dyn=$j, L= ${L[$i]}"
      ./ising -d 2 -L ${L[$i]} -T 2.25 --nmeas 1 --nmcs 100000  --ieq 2000 -s 3291043 --dyn $j > "$outdir/${L[$i]}_dyn${j}_raw.dat"
      ./mycut -f 1 < "$outdir/${L[$i]}_dyn${j}_raw.dat" > "$outdir/${L[$i]}_dyn_${j}_energy.dat"
      ./mycut -f 2 < "$outdir/${L[$i]}_dyn${j}_raw.dat" > "$outdir/${L[$i]}_dyn_${j}_magnet.dat"
      ./autocorr 3000 < "$outdir/${L[$i]}_dyn_${j}_energy.dat" > "$outdir/${L[$i]}_dyn_${j}_energy_autocorr.dat"
      ./autocorr 3000 < "$outdir/${L[$i]}_dyn_${j}_magnet.dat" > "$outdir/${L[$i]}_dyn_${j}_magnet_autocorr.dat"
      maxrangeE=${rangeE[$j]}
      maxrangeM=${rangeM[$j]}
      gnuplot -e "inputE='$outdir/${L[$i]}_dyn_${j}_energy_autocorr.dat';inputM='$outdir/${L[$i]}_dyn_${j}_magnet_autocorr.dat';outputE='$outdir/${L[$i]}_dyn_${j}_energy_autocorr.eps';outputM='$outdir/${L[$i]}_dyn_${j}_magnet_autocorr.eps';myrangeE=$maxrangeE;myrangeM=$maxrangeM" PartB3plot1.gnu
   done
done


