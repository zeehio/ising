#!/usr/bin/env gnuplot

set term push
set term postscript eps enhanced color
set output outE
set xlabel "Samples"
set ylabel "Energy"
unset key
plot input u :1 notitle w l
set output outM
set xlabel "Samples"
set ylabel "Magnetization"
plot input u :(($2)/N) notitle w l
set output

