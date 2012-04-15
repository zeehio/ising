#!/bin/bash

indir="dat/B3"
outdir="dat/B4"


mkdir -p $outdir

gcc -O3 -o autocorr autocorr.c -lm || exit 1
gcc -O3 -o tauint tauint.c || exit 1

if [ ! -d "$indir" ]; then
    echo "Executa ./PartB3.sh"
    exit 1
fi



L[0]="20"
L[1]="40"
L[2]="100"

for i in $(seq 0 2);do
   echo "# L=${L[$i]}  T=2.25" > "$outdir/tau_${L[$i]}.dat"
   echo "# Dinàmica Var tau_int tau_exp" >> "$outdir/tau_${L[$i]}.dat"
   for j in "0" "3";do
      tauintE=$(./tauint < "$indir/${L[$i]}_dyn_${j}_energy_autocorr.dat")
      tauintM=$(./tauint < "$indir/${L[$i]}_dyn_${j}_magnet_autocorr.dat")
      tauexpE=$( gnuplot -e "f(x)=exp(-x/t);fit f(x) '$indir/${L[$i]}_dyn_${j}_energy_autocorr.dat' via t; print t" 2>&1 | tail -n 1 )
      tauexpM=$( gnuplot -e "f(x)=exp(-x/t);fit f(x) '$indir/${L[$i]}_dyn_${j}_magnet_autocorr.dat' via t; print t" 2>&1 | tail -n 1 )
      echo $j "E" $tauintE $tauexpE >> "$outdir/tau_${L[$i]}.dat"
      echo $j "M" $tauintM $tauexpM >> "$outdir/tau_${L[$i]}.dat"
   done
done



