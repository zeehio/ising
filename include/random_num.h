
#ifndef RANDOM_NUM_H
#define RANDOM_NUM_H

/** Initialises a Random number generator 
 * \param[input] seed: Seed of the Random Number Generator */
void InitRNG(unsigned long int seed);

/** Frees the Random number Generator */
void FreeRNG();

/** Draws a double uniform random number between min and max: 
 * \param[input] min: Lowest value for the random number.
 * \param[input] max: Greatest value for the random number.
 * */
double drand(double min,double max);

/** Draws a random uniform integer in range [min,max) 
 * \param[input] min: Lowest value for the random number.
 * \param[input] max: First value not possible for the random number.
 * */
long int urand(long int min,long int max);


#endif

