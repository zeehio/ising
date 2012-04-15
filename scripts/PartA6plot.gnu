#!/usr/bin/env gnuplot

set term push
set term postscript eps enhanced color
set output outE
set xlabel "Samples"
set ylabel "Energy"
set style line 1 lt 1 lc 1 lw 3
set style line 2 lt 1 lc 2 lw 3
set style line 3 lt 1 lc 3 lw 3
set style line 4 lt 1 lc 4 lw 3
set style line 5 lt 1 lc 5 lw 3
set style line 6 lt 1 lc 7 lw 3
plot input1 u :(($1)/N) ls 1 t 'Simulation T=2.0' w l, input2 u :(($1)/N) ls 2 t 'Simulation T=2.5' w l, input3 u :(($1)/N) ls 3 t 'Simulation T=3.0' w l, \
     -1.74556458 ls 4 t 'Ferdinand T=2.0',-1.10607921 ls 5 t 'Ferdinand T=2.5' , -0.81730959 ls 6 t 'Ferdinand T=3.0'
set output
