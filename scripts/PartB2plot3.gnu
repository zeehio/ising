#!/usr/bin/env gnuplot

set style line 1 lt 1 lw 3
set style line 2 lt 2 lw 3
set style line 3 lt 3 lw 3
set style line 4 lt 4 lw 3

set term push
set term postscript eps enhanced color
set output outE
set xlabel "Samples"
set ylabel "Energy"
plot input1 u :(($1)/N) ls 1 t 'T=2.0' w l, input2 u :(($1)/N) ls 2 t 'T=2.5' w l, input3 u :(($1)/N) ls 3 t 'T=3.0' w l
set output outM
set key bottom
set xlabel "Samples"
set ylabel "Magnetization"
plot input1 u :(($2)/N) ls 1 t 'T=2.0' w l, input2 u :(($2)/N) ls 2 t 'T=2.5' w l,input3 u :(($2)/N) ls 3 t 'T=3.0' w l
set output

