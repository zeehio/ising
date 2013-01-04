#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

// ********************************************
// Utilities
// ********************************************

#include "random_num.h"
#include "memory.h"

// Reads all the data from stdin and stores it at x, which is expected uninitialized
int llegeixx(double **x)
{
	int veclen=1024;
	int len=0;
	double tx;
	int c;

	*x=xmalloc(veclen*sizeof(double));
	
	
	while (!feof(stdin))
	{
		if (len == veclen) // Fem el vector el doble de gran per ficar més números
		{
			veclen*=2;
			*x =xrealloc(*x, veclen*sizeof(double));
		}
		c=fgetc(stdin);
		if (c == EOF ) break;
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
			(*x)[len]=tx;
			len++;
		} else
		{
			while (fgetc(stdin)!='\n' || feof(stdin));
			fprintf(stderr,"Error at data entry %d. Ignoring a line\n",len);
		}
	}
	
	fprintf(stderr,"# %d dades llegides.\n",len);
	return len;
}

/*******************************************************************
 *  Dummy functions required by a general Jackknife implementation *
 *******************************************************************/

// Total sum of squares: tss = sum (x[i]-mean)^2
double tss_m(double *x, int stride, int n, double m)
{
	double tss=0.0;
	int i;
	for (i=0;i<n;i+=stride)
		tss+=(x[i]-m)*(x[i]-m);
	return tss;
}

// Trivial :-)
double identity(double *v)
{
	return v[0];
}

// Dummy
double myvariance(double *v)
{
	return v[0]-v[1]*v[1];
}

// Binder parameter
double binderparam(double *v)
{
	return 0.5*(3.0-v[0]/(v[1]*v[1]));
}

// Mean:
double moment1(int n,double *v)
{
	double suma=0.0;
	int i;
	for (i=0;i<n;++i)
		suma+=v[i];
	return suma/(double)n;
}

// Mean^2
double moment1sq(int n,double *v)
{
	return pow(moment1(n,v),2.0);
}

// < |X| >
double moment1abs(int n,double *v)
{
	double suma=0.0;
	int i;
	for (i=0;i<n;++i)
		suma+=fabs(v[i]);
	return suma/(double)n;
}

// < X^2 >
double moment2(int n, double *v)
{
	double suma=0.0;
	int i;
	for (i=0;i<n;++i)
		suma+=v[i]*v[i];
	return suma/(double)n;
}

// < X^3 >
double moment3(int n, double *v)
{
	double suma=0.0;
	int i;
	for (i=0;i<n;++i)
		suma+=v[i]*v[i]*v[i];
	return suma/(double)n;
}

// < X^4 >
double moment4(int n, double *v)
{
	double suma=0.0;
	int i;
	for (i=0;i<n;++i)
		suma+=v[i]*v[i]*v[i]*v[i];
	return suma/(double)n;
}
/*******************************************
 * End of dummy functions                  *
 *******************************************/

// Swap components i and j from data
void swapvalues(int i,int j, double *data)
{
	const double tmp=data[i];
	data[i]=data[j];
	data[j]=tmp;
	return;
}

/*****************************************************
 * jackknife: A general purpose jackknife estimation.
 *            It can compute the expected value and the error
 *            of ANY function depending on ANY arguments.
 * 
 * datalength: Length of data
 * data: Data to be jackknifed
 * *jack_error: A place to store the jackknife error.
 * funcargs: Arguments demanded by the function we want to estimate
 * functoest: The function we want to estimate, which may depend on the mean, 
 *            or any other parameter.
 * fun[]: Array of functions of size funcargs. This functions are used to compute the parameters passed to the 
 *        functoest. Jackknifed data is passed to each fun, and the returned result is
 *        used in the call to functoest as an argument.
 * See JackknifeEstimations for several examples.
 */
double jackknife(int datalength, double *data, double *jack_error, \
                 int funcargs, double functoest(double *),double (*fun[])(int, double*) )
{
	double *datajack = xmalloc(datalength*sizeof(double));
	double jackknife_est;
	double *arguments=xmalloc(funcargs*sizeof(double));
	double fjotesmean;
	double *fjotes=xmalloc(datalength*sizeof(double));
	int i,j;
	
	// We copy the data series:
	memcpy(datajack,data,datalength*sizeof(double));
	
	// We extract a data sample on each i-iteration
	for (i=0;i<datalength;++i)
	{
		// For each argument of the function to estimate
		// we compute the "xj" for that argument and this dataset.
		for (j=0;j<funcargs;++j)
		{
			arguments[j]=fun[j](datalength-1,datajack);// the last element is ignored. 
		}
		
		fjotes[i]=functoest(arguments);
		swapvalues(i,datalength-1,datajack); // We swap the values to prepare for the next jackknife dataset
	}
	
	for (j=0;j<funcargs;++j) // The whole dataset
	{
		arguments[j]=fun[j](datalength,datajack);
	}
	
	// Average of fj
	fjotesmean=moment1(datalength,fjotes);
	// Jackknife estimation: \tilde{f}.
	jackknife_est = datalength*functoest(arguments)-(datalength-1)*fjotesmean;
	
	// Jackknife error:
	if (jack_error != NULL)
	{
		*jack_error = sqrt((double)(datalength-1)/datalength*tss_m(fjotes,1,datalength,fjotesmean));
	}
	
	free(datajack);
	free(arguments);
	free(fjotes);
	return jackknife_est; // We return the estimated parameter
}

// A function to test jackknife
void Testjackknife()
{
	InitRNG(109);
	int N=10000;
	double *data=xmalloc(N*sizeof(double));
	int i;
	for (i=0;i<N;++i) data[i]=drand(0,1);
	double jack_error,jack;
	double (*funcs[1])(int,double*);
	funcs[0]=moment1;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("jack: %le +/- %le\n",jack,jack_error);
	
	free(data);
	FreeRNG();
	exit(0);
	return;
}

// This function performs the requested jackknife estimations.
// It is strongly based on the dummy functions defined above.
void JackknifeEstimations(int N, double *data)
{
	double jack_error,jack;
	double (*funcs[2])(int,double*);
	
	funcs[0]=moment1;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("<X> = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment1abs;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("<|X|> = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment2;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("<X^2> = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment3;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("<X^3> = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment4;
	jack=jackknife(N,data,&jack_error,1,identity,funcs);
	printf("<X^4> = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment2;
	funcs[1]=moment1;
	jack=jackknife(N,data,&jack_error,2,myvariance,funcs);
	printf("<X^2> - <X>^2 = %le +/- %le\n",jack,jack_error);
	
	funcs[0]=moment4;
	funcs[1]=moment2;
	jack=jackknife(N,data,&jack_error,2,binderparam,funcs);
	printf("Binder param g_X = %le +/- %le\n",jack,jack_error);

	return;
}

int main(int argc,char *argv[])
{
	double *dades;
	int N;
	// Read data and make estimations
	N=llegeixx(&dades);
	JackknifeEstimations(N,dades);
	
	return 0;
}
