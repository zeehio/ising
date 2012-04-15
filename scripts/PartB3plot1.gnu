#!/usr/bin/env gnuplot

set term push
set term postscript eps enhanced color
set output outputE
set xrange [0:myrangeE]
set xlabel "Sample lag"
set ylabel "Energy Autocorrelation function"
plot inputE  notitle w l
set output outputM
set xrange [0:myrangeM]
set xlabel "Sample lag"
set ylabel "Magnetization Autocorrelation function"
plot inputM  notitle w l
set output

