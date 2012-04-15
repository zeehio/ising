#!/usr/bin/env gnuplot

set term push
set term postscript eps enhanced color
set output outE
set xlabel "Samples"
set ylabel "Energy"
plot input1 u :(($1)/N) t 'T=2.0' w l, input2 u :(($1)/N) t 'T=2.5' w l, input3 u :(($1)/N) t 'T=3.0' w l
set output outM
set xlabel "Samples"
set ylabel "Magnetization"
plot input1 u :(($2)/N) t 'T=2.0' w l, input2 u :(($2)/N) t 'T=2.5' w l,input3 u :(($2)/N) t 'T=3.0' w l
set output

