#!/usr/bin/env gnuplot

set style line 1 lt 1 lw 3
set style line 2 lt 2 lw 3
set style line 3 lt 3 lw 3
set style line 4 lt 4 lw 3

set term push
set term postscript eps enhanced color
set output outE
set xrange[0:range]
set xlabel "Samples"
set ylabel "Energy"
plot inputW u :(($1)/N) ls 1 t "Wolff" w l, inputM u :(($1)/N) ls 2 t "Metropolis" w l, ex ls 3 w l t "Exact value"
set output outM
set xlabel "Samples"
set ylabel "Magnetization"
plot inputW u :(abs($2)/N) ls 1 t "Wolff" w l, inputM u :(abs($2)/N) ls 2 t "Metropolis" w l
set output

