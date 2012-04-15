#!/usr/bin/gnuplot
set term postscript eps enhanced color
set output output
set xlabel "Samples per block"
set ylabel "Variance of each blocks' mean energy"
set title "Determining {/Symbol t}_{int} for the average energy"
unset key
plot input u 1:3 notitle
set output
