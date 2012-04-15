#include "memory.h"

/** Allocate memory */
void* xmalloc(size_t midaenbytes)
{
    void* ptr=malloc(midaenbytes);
    if (ptr == NULL) abort();
    return ptr;
}

/** Changed the reserved memory size */
void* xrealloc(void* vector, size_t midaenbytes)
{
	void *tmpvec=NULL;
	tmpvec=realloc(vector,midaenbytes);
	if (tmpvec == NULL) abort();
	return tmpvec;
}


/** Function that creates an integer vector. The size of the vector i saved in the component 0. */
IntVector CreateIntVector(int size)
{
	int* v=xmalloc((size+1)*sizeof(int));
	v[0]=size;
	return (IntVector) v+1; // La component -1 del vector tindrà la seva mida :-)
}

IntVector ResizeIntVector(IntVector v, int newsize)
{
	int *ptr=--v;
	ptr=xrealloc(ptr,(newsize+1)*sizeof(int));
	ptr[0]=newsize;
	return (IntVector) ptr+1;
}
	

/** Function that frees an integer vector */
void DestroyIntVector(IntVector v)
{
	free(--v);
	return;
}


/** Function that creates an integer vector. The size of the vector i saved in the component 0. */
DoubleVector CreateDoubleVector(int size)
{
	DoubleVector v=xmalloc((size+1)*sizeof(double));
	v[0]=size;
	return v+1; // La component -1 del vector tindrà la seva mida :-)
}

DoubleVector ResizeDoubleVector(DoubleVector v, int newsize)
{
	double *ptr=--v;
	ptr=xrealloc(ptr,(newsize+1)*sizeof(double));
	ptr[0]=newsize;
	return (DoubleVector) ptr+1;
}
	

/** Function that frees an integer vector */
void DestroyDoubleVector(DoubleVector v)
{
	free(--v);
	return;
}


ListIntVector CreateListIntVector(int size)
{
	ListIntVector v=xmalloc(size*sizeof(IntVector));
	return v;
}

ListIntVector ResizeListIntVector(ListIntVector v,int oldsize, int newsize)
{
	v=xrealloc(v,newsize*sizeof(IntVector));
	return v;
}
	

/** Function that frees an array of integers */
void DestroyListIntVector(ListIntVector v, int length)
{
	int i;
	for (i=0;i<length;++i)
		DestroyIntVector(v[i]);
	free(v);
	return;
}

/** Function that sets a vector of integers to zero */
void ZeroIntVector(IntVector v)
{
	memset(v,0,v[-1]*sizeof(int));
	return;
}

/** Function that sets an array of IntVectors to zero */
void ZeroListIntVector(ListIntVector v, int N)
{
	int i;
	for (i=0;i<N;++i)
	{
		ZeroIntVector(v[i]);
	}
	return;
}
