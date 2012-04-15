/*      autocorr.c */

#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "memory.h"

DoubleVector ReadInput()
{
	int len=0;
	char c;
	double tmp;
	DoubleVector vector=CreateDoubleVector(1024);
	// Read the standard input, ignoring blank lines and lines starting with "#"
	while (!feof(stdin))
	{
		if (len == vector[-1]) // Fem el vector el doble de gran per ficar més números
		{
			vector=ResizeDoubleVector(vector,2*vector[-1]);
		}
		c=fgetc(stdin);
		if (c=='\n') continue;
		else if (c=='#')
		{
			while (fgetc(stdin)!='\n' || feof(stdin)); // fscanf("\n"); ?
			continue;
		}
		else
			ungetc(c,stdin);
		
		if (fscanf(stdin,"%lf",&tmp)==1)
			vector[len++]=tmp;
	}
	vector=ResizeDoubleVector(vector,len);
	return vector;
}

int main(int argc, char *argv[])
{
	int maxcorrlen=-1;
	int i;
	DoubleVector invector,outvector;
	
	// Read Input data from stdin:
	invector=ReadInput();
	
	// The argument is the maximum correlation length computed
	if (argc==2)
	{
		maxcorrlen=atoi(argv[1]);
	}
	
	// Validate maxcorrlen:
	if (maxcorrlen > invector[-1])
	{
		maxcorrlen=invector[-1];
	}
	if (maxcorrlen <= 0 )
	{
		 if (invector[-1] > 200)
			maxcorrlen=50;
		else
			maxcorrlen=invector[-1];
	}

	fprintf(stderr,"# Max Corr length: %d\n",maxcorrlen);


	outvector=CreateDoubleVector(maxcorrlen);
	
	// Compute the autocorrelation:	
	autocorrel(invector,outvector);
	// Print the autocorrelation:
	for (i=0;i<outvector[-1];++i)
		fprintf(stdout,"%lf\n",outvector[i]);

	// Deallocate the data
	DestroyDoubleVector(outvector);
	DestroyDoubleVector(invector);	
	return 0;
}

