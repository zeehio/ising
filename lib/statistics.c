/** Funcions estadístiques */
#include "memory.h"

// Compute the sample mean.
double mean(DoubleVector x,const int stride)
{
	double n=x[-1];
	double m=0.0;
	int i;
	for (i=0;i<(int)n;i+=stride)
		m+=x[i];
	return m/n;
}

// Compute the variance given the mean:
double variance_m(DoubleVector x,const int stride, const double mean)
{
	double n=x[-1];
	if(x==NULL)
		abort();
	if (n<=1)
		abort();
	double v=0.0;
	int i;
	for (i=0;i<(int)n;i+=stride)
		v+=(x[i]-mean)*(x[i]-mean);
	return v/(double)(n-1);
}

int autocorrel(DoubleVector dades,DoubleVector sortida)
{
	int maxcorrlen=sortida[-1];
	int mida=dades[-1];
	if (maxcorrlen <= 0 || maxcorrlen > mida)
		return -1;
	int i,j;
	
	const double mitj=mean(dades,1);
	const double var =variance_m(dades,1,mitj);
		
	for (i=0;i<mida;++i)
		dades[i]-=mitj;
	
	for (j=0;j<maxcorrlen;++j)
		sortida[j]=0.0;
	
	for (j=0;j<maxcorrlen;++j)
		for (i=0;i<mida-j;++i)
			sortida[j]+=dades[i]*dades[i+j];
	
	for (j=0;j<maxcorrlen;++j)
	{
		sortida[j]/=(mida-j-1)*var;
	}
	return 0;
}
