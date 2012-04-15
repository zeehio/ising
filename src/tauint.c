//      tauint.c
// Computes tauint from the autocorrelation data
#include <stdio.h>
#include <stdlib.h>


// Reads one double from stdin ignoring blank lines and "#" starting lines.
int llegeixx(double *x)
{
	double tx;
	int c;
	while (!feof(stdin))
	{
		c=fgetc(stdin);
		if (c == EOF ) return -1;
		if (c=='\n') continue;
		else if (c=='#')
		{
			while (fgetc(stdin)!='\n' || feof(stdin));
			continue;
		}
		else
			ungetc(c,stdin);
		
		if (fscanf(stdin,"%lf",&tx)==1)
		{
			*x=tx;
			return 0;
		} else
		{
			while (fgetc(stdin)!='\n' || feof(stdin));
			fprintf(stderr,"Error at data. Ignoring a line\n");
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	double num;
	double suma=0.0;
	
	// Integrates the autocorrelation function
	while (llegeixx(&num) == 0)
	{
		suma+=num;
	}
	// Returns tauint
	printf("%lf\n",1+2*suma);

	return 0;
}

