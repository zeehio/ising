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
plot input1 u :(($1)/N) ls 1 t 'Glauber single' w l, input2 u :(($1)/N) ls 2 t 'Glauber sequential' w l, input3 u :(($1)/N) ls 3 t 'Kawasaki' w l, -1.74556458 ls 4 w l t 'Exact value'
set key left
set output outM
set xlabel "Samples"
set ylabel "Magnetization"
plot input1 u :(abs($2)/N) ls 1 t 'Glauber single' w l, input2 u :(abs($2)/N) ls 2 t 'Glauber sequential' w l,input3 u :(abs($2)/N) ls 3 t 'Kawasaki' w l
set output

