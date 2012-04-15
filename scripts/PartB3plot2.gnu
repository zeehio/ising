#!/usr/bin/env gnuplot

set term push
set term postscript eps enhanced color
set output output1
set xrange [0:myrange]
set xlabel "Sample lag"
set ylabel "Energy Autocorrelation function"
plot input1  t t1 w l, input2 t t2 w l
