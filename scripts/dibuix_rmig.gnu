#!/usr/bin/gnuplot

set terminal png
set output "correl_fun.png"

f(x)=0
set logscale x
plot 'rmig_corr/correlR0.txt'     using 1 w l title 'it 0'    , \
     'rmig_corr/correlR1.txt'     using 1 w l title 'it 1'    , \
     'rmig_corr/correlR10.txt'    using 1 w l title 'it 10'   , \
     'rmig_corr/correlR100.txt'   using 1 w l title 'it 100'  , \
     'rmig_corr/correlR1000.txt'  using 1 w l title 'it 1000' , \
     'rmig_corr/correlR10000.txt' using 1 w l title 'it 10000', \
     'rmig_corr/correlR100000.txt' using 1 w l title 'it 100000', \
          f(x)                    title '0'

