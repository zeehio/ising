#!/bin/bash

outdir="dat/A3"
data="$outdir/7.dat"
fitxenergies="$outdir/7_energies.dat"
outfile="$outdir/energies.dat"
outplot="$outdir/energies.eps"
PG=
#PG=" -pg "

gcc -O3 -o binning binning.c -lm
gcc -O3 $PG -o ising ising.c -lm

mkdir -p $outdir

rm -f $outfile

./ising -d 2 -L 20 -T 2.0 --nmeas 1 --nmcs 10000000  --ieq 1000 -s 107 > "$outdir/7.dat"

#./ising -d 2 -L 20 -T 2.0 --nmeas 1 --nmcs 100000000  --ieq 1000 -s 107 > "$outdir/8.dat"
#./ising -d 2 -L 20 -T 2.0 --nmeas 1 --nmcs 1000000000  --ieq 1000 -s 107 > "$outdir/9.dat"
N=400

./mycut -f 1 < "$data" > "$fitxenergies"

m=1
for i in {1..12};do
	cat $fitxenergies | ./binning -m $m | head -n 1 | sed 's/#//' >> "$outfile"
	m=$( expr $m \* 2)
done

gnuplot -e "input='$outfile';output='$outplot'" ./PartA3plot.gnu

m=400
mEmeanEvar=$(cat $fitxenergies | ./binning -m $m | head -n 1 | sed 's/#//' | sed 's/\t/ /g' )
Emean=$(echo ${mEmeanEvar} | cut -d " " -f 2)
Evar=$(echo  ${mEmeanEvar} | cut -d " " -f 3)


error=$( echo "scale=8;sqrt($Evar)/$N" | bc)
fisher=$( echo "scale=8;$Emean/$N" | bc)
echo "Fisher: <E>/N = $fisher +/- $error" | tee "$outdir/fisher.dat"

