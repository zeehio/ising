
#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <string.h> // memset

// We define a variable type "Integer vector"
typedef int* IntVector;
typedef IntVector* ListIntVector;
typedef double* DoubleVector;


/** Allocate memory */
void* xmalloc(size_t midaenbytes);

/** Changed the reserved memory size */
void* xrealloc(void* vector, size_t midaenbytes);


/** Function that creates an integer vector. The size of the vector i saved in the component 0. */
IntVector CreateIntVector(int size);

IntVector ResizeIntVector(IntVector v, int newsize);

/** Function that frees an integer vector */
void DestroyIntVector(IntVector v);

/** Function that creates an integer vector. The size of the vector i saved in the component 0. */
DoubleVector CreateDoubleVector(int size);

DoubleVector ResizeDoubleVector(DoubleVector v, int newsize);

/** Function that frees an integer vector */
void DestroyDoubleVector(DoubleVector v);

ListIntVector CreateListIntVector(int size);
ListIntVector ResizeListIntVector(ListIntVector v,int oldsize, int newsize);
/** Function that frees an array of integers */
void DestroyListIntVector(ListIntVector v, int length);


/** Function that sets a vector of integers to zero */
void ZeroIntVector(IntVector v);

/** Function that sets an array of IntVectors to zero */
void ZeroListIntVector(ListIntVector v, int N);


#endif

