#!/bin/bash

indir="dat/A5"

if [ ! -d "$indir" ]; then
   echo "Please run ./PartA5.sh first."
   exit 1
fi

outdir="dat/A6"

mkdir -p $outdir

Dirferdi="Ferdinand/ForProg"
if [ ! -d $Dirferdi ];then
   echo "Ferdinand not found"
   exit 1
fi

currdir=$PWD
cd "$Dirferdi"
gfortran -Wall -o ferdinand ferdinand.f || exit 1
cd $currdir

echo "Ferdinand output:" > "$outdir/ferdinand.dat"
./$Dirferdi/ferdinand 100 2.0 100 2.5 100 3.0 >>  "$outdir/ferdinand.dat"
echo "" >> "$outdir/ferdinand.dat"
./$Dirferdi/ferdinand 20 2.0 20 2.5 20 3.0 >> "$outdir/ferdinand.dat"


N=10000.0

gnuplot -e "N=$N;input1='$indir/2.0_trans.dat';input2='$indir/2.5_trans.dat';input3='$indir/3.0_trans.dat';outE='$outdir/all_trans_E.eps'" PartA6plot.gnu 

