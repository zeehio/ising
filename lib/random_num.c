/** Generador de números aleatoris */

#include "r1279.h"

/** Initialises a Random number generator */
void InitRNG(unsigned long int seed) {
    setr1279(seed);
    return;
}
/** Frees the Random number Generator: */
void FreeRNG() {
	return;
}
/** Draws a double uniform random number between min and max: */
double drand(double min,double max)
{
    return min + r1279()*(max-min);
}
/** Draws a random uniform integer in range [min,max) */
long int urand(long int min,long int max) {
    return ir1279range(min,max-1);
}
