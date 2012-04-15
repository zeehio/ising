#!/usr/bin/env gnuplot

set style line 1 lt 1 lw 3

set term push
set term postscript eps enhanced color
set output outE
set xlabel "Samples"
set ylabel "Energy"
unset key
plot input u :(($1)/N) ls 1 notitle w l
set output outM
set xlabel "Samples"
set ylabel "Magnetization"
plot input u :(abs($2)/N) ls 1 notitle w l
set output

