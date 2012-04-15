#!/bin/bash

outdir="dat/B1"
data="$outdir/5.dat"
fitxenergies="$outdir/5_energies.dat"
#outfile="$outdir/energies.dat"
#outplot="$outdir/energies.eps"
PG=
#PG=" -pg "

mkdir -p $outdir

gcc -O3 -o binning binning.c -lm || exit 1
gcc -O3 $PG -o ising ising.c -lm || exit 1

./ising -d 2 -L 20 -T 2.0 --nmeas 1 --nmcs 1000000  --ieq 1000 -s 3291043 --dyn 3 > "$outdir/5.dat"
N=400.0

./mycut -f 1 < "$data" > "$fitxenergies"


m=200
mEmeanEvar=$(cat $fitxenergies | ./binning -m $m 2>/dev/null | head -n 1 | sed 's/#//' | sed 's/\t/ /g' )
Emean=$(echo ${mEmeanEvar} | cut -d " " -f 2)
Evar=$(echo  ${mEmeanEvar} | cut -d " " -f 3)


error=$( echo "scale=8;sqrt($Evar)/$N" | bc)
fisher=$( echo "scale=8;$Emean/$N" | bc)
echo "Fisher teoria: <E>/N = -1.7455571250" | tee "$outdir/fisher.dat"
echo "Fisher: <E>/N = $fisher +/- $error" | tee  -a "$outdir/fisher.dat"

