#!/bin/bash

outdir="dat/B2"
PG=
#PG=" -pg "

mkdir -p $outdir

gcc -O3 $PG -o ising ising.c -lm

exact[1]=-1.74556458
exact[2]=-1.10607921
exact[3]=-0.81730959
Temp[1]=2.0
Temp[2]=2.5
Temp[3]=3.0
rang[1]=1000
rang[2]=100
rang[3]=100

for i in "1" "2" "3"; do
   T=${Temp[$i]}
   ./ising -d 2 -L 100 -T $T --nmeas 1 --nmcs 1001  --ieq 1 -s 3291043 --dyn 3 > "$outdir/${T}_raw.dat"
   N=10000
   ex=${exact[$i]}
   range=${rang[$i]}
   gnuplot -e "N=$N;input='$outdir/${T}_raw.dat';outE='$outdir/${T}_trans_E.eps';outM='$outdir/${T}_trans_M.eps'" PartB2plot1.gnu
   gnuplot -e "N=$N;ex=$ex;range=$range;inputW='$outdir/${T}_raw.dat';inputM='dat/A5/${T}_trans.dat';outE='$outdir/${T}_trans_E_WM.eps';outM='$outdir/${T}_trans_M_WM.eps'" PartB2plot2.gnu
done


gnuplot -e "N=$N;input1='$outdir/2.0_raw.dat';input2='$outdir/2.5_raw.dat';input3='$outdir/3.0_raw.dat';outE='$outdir/all_trans_E.eps';outM='$outdir/all_trans_M.eps'" PartB2plot3.gnu 
