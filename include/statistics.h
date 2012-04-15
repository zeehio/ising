
#ifndef STATISTICS_H
#define STATISTICS_H

#include "memory.h"

// Compute the mean
double mean(DoubleVector x,const int stride);

// Compute the variance given the mean:
double variance_m(DoubleVector x,const int stride, const double mean);

int autocorrel(DoubleVector dades,DoubleVector sortida);

#endif
