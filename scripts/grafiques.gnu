#!/usr/bin/gnuplot

dades='dades.txt'
out1=sprintf('%s_Energies.png',dades)
out2=sprintf('%s_Magnet.png',dades)
out3=sprintf('%s_Radi.png',dades)
out4=sprintf('%s_Radi2.png',dades)

set terminal png
set xlabel 'Iteracions'
set output out1
plot dades  using 1 with lines title 'Energia' 
set output out2
plot dades  using 2 with lines title 'Magnetització'
set output out3
set logscale xy
plot dades  using 3 with lines title 'Radi mig'

set output out4
unset logscale
f(x)=a*x+b
fit f(x) dades using (log($0)):(log($3)) via a,b
titolnostre=sprintf('a*x+b, a=%f, b=%f',a,b)
plot dades using (log($0)):(log($3)) title 'Dades Radi mig' , f(x) title titolnostre
