//      binning2.c

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include <getopt.h>


#include "statistics.h"
#include "memory.h"	

// Read an integer from stdin and store at x. Ignore blank lines and lines starting with "#"
int llegeixx(int* x)
{
	int tx,c;
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
		
		if (fscanf(stdin,"%d",&tx)==1)
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

/** Help function. */
void help()
{
	fprintf(stderr,"===============================================\n");
	fprintf(stderr,"binning: A tiny utility to bin data\n");
	fprintf(stderr," Usage:\n");
	fprintf(stderr,"  binning [ -m 200 ] < filein > fileout \n");
	fprintf(stderr," Help:\n");
	fprintf(stderr,"  -m [200]: Number of data on each block\n");
	fprintf(stderr," Example:\n");
	fprintf(stderr,"  binning -m 50 < mydata.dat > databinned.dat\n");
	return;
}

// Parse the command line arguments using getopt.
int parseinput(int argc, char **argv,int *m)
{
    int c,itmp;
    opterr=0;
    while ((c = getopt (argc, argv, "m:h")) != -1)
    {
		switch (c)
		{
		case 'h':
			help();
			break;
		case 'm':
			itmp=atoi(optarg);
			if (itmp>0)
			{
				*m=itmp;
				fprintf(stderr, "# Bin size: %d\n",*m);
			}
			else
				fprintf(stderr,"# Invalid bin number: %d. Using %d instead\n",itmp,*m);
			break;
		case '?':
			if (optopt == 'n' )
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
			abort();
		default:
			abort ();
		}
    }
    return 0;
}

int main(int argc, char **argv)
{
	int m=200;
	int nblocks=0;
	int tx;
	parseinput(argc,argv,&m); // Parse commmand line arguments
	int i;
	DoubleVector xm=CreateDoubleVector(1024);
	double xmean,xmeanvar;
	int end=0;
	// Create an array to store the blocks of the binning

	for (nblocks=0;end==0;++nblocks)
	{
		if (nblocks == xm[-1]) // We need more space.
		{
			xm=ResizeDoubleVector(xm,2*xm[-1]);
		}
		xm[nblocks]=0.0;
		for (i=0;i<m;++i)
		{
			if (llegeixx(&tx) == -1)
			{
				end=1;
				break;
			}
			xm[nblocks]+=tx;
		}
	}
	xm=ResizeDoubleVector(xm,nblocks-1);
	for (i=0;i<xm[-1];++i)
	{
		xm[i]/=(double)m;
	}
	
	// Print mean and variance of the data (commented)
	xmean=mean(xm,1);
	xmeanvar=variance_m(xm,1,xmean)/(double)xm[-1];
	fprintf(stdout,"# %d\t%f\t%f\n",m,xmean,xmeanvar);
	
	// Print the binned data:
	for (i=0;i<xm[-1];++i)
		printf("%le\n",xm[i]);
	DestroyDoubleVector(xm);
	return 0;
}

