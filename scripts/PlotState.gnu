#!/usr/bin/gnuplot

if (!exists('fitx') || strlen(fitx)==0) fitx='./estats/estat0.txt' 
if (!exists('out1') || strlen(out1)==0) out1='./plot.eps' 
set terminal postscript eps enhanced color
set output out1
set palette grey
set xrange [0:]
set yrange [0:]
plot fitx matrix with image

