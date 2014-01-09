//      ising.c
// Per compilar:
// gcc -o ising ising.c  -lm 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

#include "memory.h"
#include "fileio.h"
#include "statistics.h"
#include "random_num.h"

// Variable to store the exponential:
static double *ising_exp=NULL;


/** Function that shows by screen the help menu. */
void help()
{
    fprintf(stderr,"===============================================\n");
    fprintf(stderr,"Ising model simulation on a hypercubic lattice:\n");
    fprintf(stderr," Usage:\n");
    fprintf(stderr,"  ising [ -d 4 ] [ -L 20 ] [ -T 4.0 ] [ [-m | --nmeas ] 20 ] [ [ -c | --nmcs ] 10000000 ] \n");
    fprintf(stderr," Help:\n");
    fprintf(stderr,"  -d [2]: Lattice dimension\n");
    fprintf(stderr,"  -L [20]: Lattice side length\n");
    fprintf(stderr,"  -T [2.0]: Temperature of the simulation\n");
    fprintf(stderr,"  --nmeas [1]: Number of MonteCarlo samples between two measures \n");
    fprintf(stderr,"  --nmcs [10000000]: Total number of MonteCarlo samples\n");
    fprintf(stderr,"  -s [107]: Random Number generator seed\n");
    fprintf(stderr,"  --ieq [1000]: Extra iterations to reach equilibrium\n");
    fprintf(stderr,"  --dyn [0]: Dynamics used (Glauber: 0, Glauber sequential: 1, Kawasaki (random pairs):2, Wolff: 3, Kawasaki (neighbour pairs): 4\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"===============================================\n");
    fprintf(stderr,"Options for 2D square lattices:\n");
    fprintf(stderr,"  --print-state: Prints the state n the \"state/\" subdirectory\n");
    fprintf(stderr,"===============================================\n");
    fprintf(stderr," Example:\n");
    fprintf(stderr,"  ising -d 4 -L 20 -T 4.0 --nmeas 20 --nmcs 10000000 --ieq 5000 --dyn 3\n");
    return;
}

/** Function that analizes the arguments of the program and saves them 
 * d: dimension of the lattice
 * L: nº of spins at one side of the lattice
 * T: Temperature of the simulation
 * nmeas: Number of MCS between to measures
 * nmcs: Total number of MCS in our simulation
*/
int parseinput(int argc, char **argv,int *d, int *L, double *T, \
               int *nmeas,unsigned long int *nmcs, \
               unsigned long int *seed,int *ieq, int *TypeDyn, int *printstate)
{
    int c;
    int itmp;
    unsigned long int ulitmp;
    double dtmp;
    while (1)
    {
        static struct option long_options[] =
        {
            {"d",  required_argument, 0, 'D'},
            {"L",  required_argument, 0, 'L'},
            {"T",    required_argument, 0, 'T'},
            {"nmeas",    required_argument, 0, 'm'},
            {"nmcs",    required_argument, 0, 'c'},
            {"S", required_argument, 0, 's'},
            {"ieq", required_argument, 0, 'e'},
            {"dyn", required_argument, 0, 'y'},
            {"help", 0, 0, 'h'},
            {"print-state", 0, 0, 'p'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long (argc, argv, "hpD:d:l:L:T:t:m:c:s:y:", long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c)
        {
            case 'y':
                if(sscanf(optarg, "%d", &itmp) != 1)
                    fprintf(stderr,"Error parsing --dyn option\n");
                else if(itmp<0 || itmp>4)
                    fprintf(stderr,"--dyn option should be 0-4\n");
                else
                    *TypeDyn=itmp;
                break;
            
            case 'D':
            case 'd':
                if(sscanf(optarg, "%d", &itmp) != 1)
                    fprintf(stderr,"Error parsing -d option\n");
                else
                    *d=itmp;
                break;
            case 'L':
            case 'l':
                if(sscanf(optarg, "%d", &itmp) != 1)
                    fprintf(stderr,"Error parsing -L option\n");
                else
                    *L=itmp;
                break;
            case 'T':
            case 't':
                if(sscanf(optarg, "%lf", &dtmp) != 1)
                    fprintf(stderr,"Error parsing -T option\n");
                else
                    *T=dtmp;
                break;
            case 'm':
                if(sscanf(optarg, "%i", &itmp) != 1 )
                    fprintf(stderr,"Error parsing -m option\n");
                else
                    *nmeas=itmp;
                break;
            case 'c':
                if( sscanf(optarg, "%lud", &ulitmp) != 1 )
                    fprintf(stderr,"Error parsing --nmcs option\n");
                else
                    *nmcs=ulitmp;
                break;
            case 's':
                if( sscanf(optarg, "%lui", &ulitmp) != 1 )
                    fprintf(stderr,"Error parsing -s option\n");
                else
                    *seed=ulitmp;
                break;
            case 'e':
                if(sscanf(optarg, "%i", &itmp) != 1 )
                    fprintf(stderr,"Error parsing --ieq option\n");
                else
                    *ieq=itmp;
                break;
            case 'p':
                *printstate=1;
                break;
            case 'h':
                help();
                exit(-1);
                break;
            case '?':
                /* getopt_long already printed an error message. */
                break;
            default:
                abort();
        }
    }
    if (*d!=2 && *printstate==1)
    {
        fprintf(stderr,"States will not be printed as dimension is %d and not 2,\n",*d);
        *printstate=0;
    }
    return 0;
}


/** Function that gives us the index of a spin given its coordinates */
int xyz2i(const int d, const int L, int *coord)
{
    int i;
    int index=0;
    for (i=0;i<d;++i)
    {
        index+=pow(L,d-i-1)*coord[i];
    }
    return index;
}

/** Function that gives the coordinates for each spin given its number, d and L */
void i2xyz(const int d, const int L, int index, int *coord)
{
    int i;
    for (i=0;i<d;++i)
    {
        coord[i]=index/(int)pow(L,d-i-1);
        index=index-(int)pow(L,d-i-1)*coord[i];
        index=index%((int)pow(L,d-i-1));
    }
    return;
}

/** Function that initializes the lattice */
void InitHypercubicLattice(const int d,const int L,ListIntVector veins)
{
    const int N=pow(L,d); //Number of spins
    int i,j;
    int coord[d];
    IntVector vei;
    for (i=0;i<N;++i)
    {
        veins[i]=CreateIntVector(2*d); // In a hypercubic lattice, each spin has 2*d neighbors
        vei=veins[i];
        i2xyz(d,L,i,coord); // Compute the coordinates of this spin
        for (j=0;j<d;++j)
        {
            coord[j]=(coord[j]+1)%L; // Sum 1 to the coordinate j.
            vei[2*j]=xyz2i(d,L,coord); // Neighbor to the index corresponding to this coordinate. 
            coord[j]=(coord[j]-2+L)%L; // Minus 2
            vei[2*j+1]=xyz2i(d,L,coord); // Neighbor
            coord[j]=(coord[j]+1)%L; // Sum 1 (return to the initial position).
        }
    }
    return;
}

/** Create the bonds between neighbors */
void InitBonds(int N, ListIntVector veins, ListIntVector bonds)
{
    int i;
    IntVector vei;
    
    for (i=0;i<N;++i)
    {
        vei=veins[i];
        bonds[i]=CreateIntVector(vei[-1]);
        ZeroIntVector(bonds[i]);
    }
}

/** Function that initializes the spins in vector v to -1 o +1. */
void RandomVector(IntVector v)
{
    int i;
    for (i=0;i<v[-1];++i)
    {
        v[i]=2*urand(0,2)-1;
    }
    return;
}

/** Function that sets a sequence of spins for Glauber sequential updating */
void InitSequential (IntVector seq)
{
    int i,j;
    for (i=0;i<seq[-1];++i)
    {
        seq[i]=urand(0,seq[-1]); //Spin at random
        for (j=0;j<i;++j) //We look that this spin wasn't already chosed
        {
            if (seq[i] == seq[j])
            {
                --i;
                break;
            }
        }
    }
}

/** Function that print the neighbors in case we want to verify the lattice */
void ImprimeixVeins(int N,ListIntVector veins)
{
    int i,j;
    IntVector v;
    for (i=0;i<N;++i)
    {
        v=veins[i];
        printf("%d: ",i);
        for (j=0;j<v[-1];++j)
        {
            printf("%d",v[j]);
            if (j+1 != v[-1]) printf(", ");
        }
        printf("\n");
    }
    return;    
}



/** Test function of the lattice */
void Testindex2xyz()
{
    int d=3,L=3;
    int coord[d];
    int N=pow(L,d);
    int j,i;
    printf("Provant funcio i2xyz i reciproca\n");
    for (i=0;i<N;++i)
    {
        i2xyz(d,L,i,coord);
        j=xyz2i(d,L,coord);
        printf("%d: (%d, %d, %d) %d\n",i,coord[0],coord[1],coord[2],j);
    }
    return;
}
/** Flips "flip"-th spin and returns the energy difference deltaE=Eafter-Ebefore */
int FlipSpinGlauber(IntVector spins,ListIntVector veins,int flip)
{
    int deltaE=0;
    int i;
    IntVector vei=veins[flip];
    spins[flip]*=-1; // Flip spin
    for (i=0;i<vei[-1];++i) // For each neighbor, compute the contribution to the energy of the new one to the old one.
    {
        deltaE+=spins[vei[i]];
    }
    return -2*spins[flip]*deltaE;  
}

/** Flips two spins at "flips[2]" and returns the energy difference deltaE */
int FlipSpinKawasaki(IntVector spins,ListIntVector veins,int flips[])
{
    int changes=2;
    int dE[changes];
    int deltaE=0;
    int i,j;
    IntVector vei;
    for (j=0;j<changes;++j)
    {
        spins[flips[j]]*=-1; //Flip the spin
        dE[j]=0;
    }
    for (j=0;j<changes;++j)
    {
        vei=veins[flips[j]]; // vei conté els veins de l'spin a girar
        for (i=0;i<vei[-1];++i) //Compute the energy for each neighbor
        {
            if (vei[i] != flips[(j+1)%changes]) //Except if one neighbor is the other spin to flip
                dE[j]+=spins[vei[i]];
        }
    }
    for (j=0;j<changes;++j)
    {
        deltaE+=-2*spins[flips[j]]*dE[j];
    }
    
    return deltaE;  
}

/** Function that calls another function to flip spins corresponding to the dynamics we want */
int FlipSpin(IntVector spins, ListIntVector veins, int flip, int flip2, int TypeDynamics)
{
    int deltaE;
    int flips[2];
    if (TypeDynamics == 2 || TypeDynamics == 4) //Kawasaki dynamics
    {
        flips[0]=flip;
        flips[1]=flip2;
        deltaE=FlipSpinKawasaki(spins,veins,flips);
    }
    else //Glauber single-spin (TypeDynamics == 0) or Glauber sequential (TypeDynamics == 1)
    {
        deltaE=FlipSpinGlauber(spins,veins,flip);
    }
    return deltaE;
}

/** Function that flips an entire cluster for Wolff algorithm */
void FlipCluster(IntVector spins, IntVector cluster)
{
    int i;
    for (i=0;i<spins[-1];++i)
    {
        if (cluster[i] == 1) //Pertany al cluster a girar
            spins[i]*=-1;
    }
}

/** Function that computes the energy of the entire lattice*/
int Energy(IntVector spins,ListIntVector veins)
{
    int en=0;
    int i,j;
    IntVector vei;
    for (i=0; i<spins[-1];++i)
    {
        vei=veins[i];
        for (j=0;j<vei[-1];++j)
        {
            if (i<vei[j])
                en+=spins[i]*spins[vei[j]];
        }
    }
    return -en;
}

/**  Function that computes the magnetization of the entire lattice */
int Magnetization(IntVector spins)
{
    int mag=0;
    int i;
    for (i=0;i<spins[-1];++i)
        mag+=spins[i];
    return mag;
}

int SpinToFlipGlauberRandom(int Nspins, int *flip)
{
    *flip=urand(0,Nspins);
    return 1;
}

int SpinToFlipGlauberSequential(IntVector seq,int nMCSstep, int *flip)
{
    *flip=seq[nMCSstep];
    return 1;
}

int SpinToFlipKawasakiRandom(IntVector spins, int *flip, int *flip2,int itmax)
{
    int count=0;
    int Nspins=spins[-1];
    int maxkaw=Nspins*Nspins; 
    int extracounts=-1;
    *flip=urand(0,Nspins); //One spin
    do
    {
        if (count > maxkaw)
        {
            *flip=urand(0,Nspins); //Choose another spin in case we don't find spins with opposite values
            count=0;
        }
        *flip2=urand(0,Nspins-1); //And another spin
        if (*flip2 >= *flip)
            ++*flip2;
        ++count; //Count in case all the spins we choose have the same value
        ++extracounts;
    }
    while(spins[*flip] == spins[*flip2] && (itmax==-1 || extracounts < itmax)); //The two must have different value
    if (itmax==-1)
        return 0;
    else
        return extracounts;
}


int SpinToFlipKawasakiNeighbours(IntVector spins,ListIntVector veins,int* flip,int* flip2,int itmax)
{
    int extracount=-1;
    IntVector vei;
    do
    {
        *flip=urand(0,spins[-1]); //One spin
        vei=veins[*flip];
        *flip2=vei[urand(0,vei[-1])];
        extracount++;
    } while (spins[*flip] == spins[*flip2] && (itmax==-1 || extracount < itmax));
    
    if (itmax==-1)
        return 0;
    else
        return extracount;
}

/** Function that checks if the values of E,M are equal to the values computed by all the lattice
 * We use this during the writing of the code to verify it
 * In the final code this function is not used*/
int CheckEnergyAndMag(IntVector spins, ListIntVector veins, int E, int M)
{
    int newE,newM,stat=0;
    newE=Energy(spins,veins);
    newM=Magnetization(spins);
    if (E!=newE)
    {
        fprintf(stderr,"# Aargh! Energia donada (%d) diferent de real (%d)\n",E,newE);
        ++stat;
    }
    if (M!=newM)
    {
        fprintf(stderr,"# Aargh! Magnetització donada (%d) diferent de real (%d)\n",M,newM);
        stat+=2;
    }
    fflush(stderr);
    return stat;
}

/** Function that computes the table of exponentials */
void Initmyexp(int N, ListIntVector veins,double T, int TypeDyn)
{
    // 2*maxarg_i((veins[i])[-1])+1 >> E=-sum_veins si*sj; E_i = - si sum_veins{i} sj 
    // E_i =- si (N_{i+}-N_{i-}) = - si (N_{i+} - (N_{i veins}- N_{i+} ) = - si ( 2 N{i+} - N{i veins})
    int i;
    // Trobem l'spin amb més veins:
    int nmaxveins =0;
    for (i=0;i<N;++i)
    {
        if ((veins[i])[-1] > nmaxveins)
            nmaxveins = (veins[i])[-1];
    }
    // Si l'spin amb més veins fos 4 veïns, llavors
    switch (TypeDyn)
    {
        case 0: // Glauber
        case 1: // Glauber sequential
            // Necessitem de un a nmaxveins
            ising_exp=CreateDoubleVector(nmaxveins);
            for (i=0;i<nmaxveins;++i)
            {
                ising_exp[i]=exp(-(2*(i+1))/T);
            }
            break;
        case 2: // Kawasaki
        case 4:
            ising_exp=CreateDoubleVector(2*nmaxveins);
            ising_exp[0]=2*nmaxveins;
            for (i=0;i<2*nmaxveins;++i)
            {
                ising_exp[i]=exp(-(2*(i+1))/T); // deltaE = 2*(-nmaxveins+i-1) --> deltaE/2 = -nmaxveins+i-1 --> deltaE/2+1+nmaxveins=i
            }
            break;
        break;
        default:
        break;
    }

    return;
}
/** Function that frees the table of exponential */
void Destroymyexp()
{
    DestroyDoubleVector(ising_exp);
    return;
}

/** Function that gives a value of exponential from the table of exponentials */
double myexp(int deltaE, int TypeDyn)
{
    int i=deltaE/2-1; // Tant per Kawasaki com per Glauber, aquest criteri serveix.
    return ising_exp[i];
}

/** Function that gives us if the bond is occupied (1) or not occupied (0)*/
int OccupyBondProb(int Si, int Sj, double expo)
{
    int delta=abs((Si+Sj)/2.0);
    if (delta==0 || drand(0.0,1.0) > (1.0-expo)) 
        return 0;
    else
    {
        return 1;
    }
}

/** Function that sets a bond between two spins at the value (usually 1)*/
void SearchBond(IntVector vei, IntVector bond, int spin, int value)
{
    int j;
    for (j=0;j<vei[-1];++j)
    {
        if (spin == vei[j])
            bond[j]=value;
    }
}

/** Function that, for each bound around the spin "around", compute if it is occupied or not*/
void BoundsAround(IntVector spins, IntVector cluster, ListIntVector veins, ListIntVector bonds, int around, double expo)
{
    int i;
    
    IntVector vei=veins[around]; 
    IntVector bond=bonds[around];
    
    for (i=0;i<vei[-1];++i)
    {
        if (bond[i]==0 && (cluster[vei[i]] == 1 || OccupyBondProb(spins[around],spins[vei[i]],expo) == 1)) 
        //Ocupem els bonds amb els veins?
        {
            bond[i] = 1;
            //Busquem ara el bond invers per tal de ficar-lo també a 1
            SearchBond(veins[vei[i]],bonds[vei[i]],around,1);
        }
    }
}

/** This recursive function calls itself each time it finds a new spin inside the cluster
 * First, we put the spin inside the cluster
 * Then, we search for occupied bonds between it and its neighbors
 * Finally, for each occupied bond, we move to the corresponding neighbor, EXCEPT when we already visited it.  
 * */

void ClusteringNeigh(IntVector spins, IntVector cluster, ListIntVector veins, ListIntVector bonds, double expo, int spin)
{
    int i;
    IntVector vei,bond;
    
    vei=veins[spin];
    bond=bonds[spin];
    
    cluster[spin]=1; //The spin is added to the cluster
    BoundsAround(spins,cluster,veins,bonds,spin,expo); //Search for occupied bonds of this spin
    
    for (i=0;i<vei[-1];++i)
    {
        if (bond[i] == 1 && cluster[vei[i]] == 0) //For each bond occupied and unvisited neighbors...
        {
            ClusteringNeigh(spins,cluster,veins,bonds,expo,vei[i]);
        }
    }
}

/** Algorithm for creating clusters */
int Wolff(IntVector spins, IntVector cluster, ListIntVector veins, ListIntVector bonds, double expo)
{
    int inicial=urand(0,spins[-1]); //Random spin from which we begin the clustering
    
    ClusteringNeigh(spins,cluster,veins,bonds,expo,inicial);
    
    return -spins[inicial]; //Value of the future spins of the cluster
}

/** Function that computes the energy of the cluster*/
int EnergyCluster(IntVector spins, IntVector cluster, ListIntVector veins, ListIntVector bonds, int spcluster, int *mag)
{
    int i,j;
    int deltaE=0,magcluster=0;
    IntVector bond,vei;
    
    for (i=0;i<cluster[-1];++i)
    {
        if (cluster[i] == 1) //Search for members of the cluster
        {
            ++magcluster;
            bond=bonds[i];
            vei=veins[i];
            for (j=0;j<vei[-1];++j) //For each, we look at the frontier
            {
                if (bond[j] == 0) //If we are at the frontier, compute energy
                {
                    deltaE+=spins[vei[j]];
                }
            }
        }
    }
    *mag+=2*magcluster*spcluster;
    return -2*deltaE*spcluster;
}

/** Function that perform "steps" clusterings. Returns E0 and M as the change in energy and magnetization */
int WolffMetropolis(IntVector spins, ListIntVector veins, ListIntVector bonds, double T, int steps, int *M)
{
    int E0=0;
    int spinscluster;
    int stp;
    int N=spins[-1];
    double expo=exp(-2.0/T);
    IntVector cluster=CreateIntVector(N);
    int totalspins=0,i;

    for (stp=0;stp<steps;++stp)
    {
        for (totalspins=0;totalspins<N;) //We flip, at least, N spins
        {
            ZeroListIntVector(bonds,N); //Sets all bonds to 0 (unoccupied)
            ZeroIntVector(cluster); //Sets the cluster values to 0 (no spins in the cluster)
            spinscluster=Wolff(spins,cluster,veins,bonds,expo); //Create the cluster
            for (i=0;i<N;++i) totalspins+=cluster[i]; //Compute the spins in the cluster
            E0+=EnergyCluster(spins,cluster,veins,bonds,spinscluster,M); //Change in energy
            FlipCluster(spins,cluster); //Flip the cluster
        }
    }
    DestroyIntVector(cluster);
    return E0;
}

/** Function that perform Metropolis for Glauber and Kawasaki dynamics, "steps" steps, and returns E0 and M
 * as the change in energy and magnetization */
int StandardMetropolis(IntVector spins, ListIntVector veins, IntVector sequential, double T,int steps, int *M, int TypeDyn)
{
    int E0=0,deltaE;
    int flip,flip2,i,stp;
    int itmaxpermcs=spins[-1];
    for (stp=0;stp<steps;++stp)
    {
        for (i=0;i<itmaxpermcs;++i) 
        {
            switch (TypeDyn)
            {
                case 0:
                    SpinToFlipGlauberRandom(spins[-1],&flip);
                    break;
                case 1:
                    SpinToFlipGlauberSequential(sequential,i,&flip);
                    break;
                case 2:
                    i+=SpinToFlipKawasakiRandom(spins,&flip,&flip2,-1);
//                    i+=SpinToFlipKawasakiRandom(spins,&flip,&flip2,itmaxpermcs-i);
                    break;
                case 4: // Kawasaki neighbours
                    i+=SpinToFlipKawasakiNeighbours(spins,veins,&flip,&flip2,-1);
//                    i+=SpinToFlipKawasakiNeighbours(spins,veins,&flip,&flip2,itmaxpermcs-i);
                    break;
                default:
                    fprintf(stderr,"Unknown dynamic in StandardMetropolis\n");
                    exit(-1);
            }
            if (i>=itmaxpermcs)
            {
                break;
            }
            deltaE=FlipSpin(spins,veins,flip,flip2,TypeDyn); //Compute the variation of energy
            if (deltaE<=0 || myexp(deltaE, TypeDyn) > drand(0,1)) //If we accept the flip...
            {
                E0+=deltaE;
                if (TypeDyn == 0 || TypeDyn == 1) //For Glauber dynamics: the magnetization changes two units.
                    *M+=2*spins[flip]; 
            }
            else //If we don't accept the flip...
            {
                FlipSpin(spins,veins,flip,flip2,TypeDyn); //We undo the change
            }
        }
    }
    return E0;
}

/** Performs 'steps' MCS, i.e. steps*N attempted spin flips. Call Wolff or Metropolis */
int Metropolis(IntVector spins,ListIntVector veins, IntVector sequential, ListIntVector bonds, double T,int steps, int *M,\
 int TypeDyn) 
{
    int E0;
    switch(TypeDyn)
    {
        case 3:
            E0=WolffMetropolis(spins,veins,bonds,T,steps,M);
            break;
        default:
            E0=StandardMetropolis(spins,veins,sequential,T,steps,M,TypeDyn);
            break;
    }
    return E0;
}

double ClusterRadius(IntVector spins,int d, int L)
{
    int i,j;
    int spinara;
    int ncanvis=0;
    double r=0.0;
    int coord[d];
    if (d != 2)
        return -1;
    
    for (i=0;i<L;++i)
    {
        coord[0]=0;
        coord[1]=i;
        spinara=spins[xyz2i(d,L,coord)];
        for (j=1;j<L;++j)
        {
            coord[0]=j;
            if (spins[xyz2i(d,L,coord)] != spinara)
            {
                ncanvis++;
                spinara*=-1;
            }
        }
    }
    r=(double)L*L/ncanvis;
    return r;
}

void CorrelRadi(int iter, IntVector spins, int L, int d)
{
    if (d!=2)
        return;
    FILE *fitx;
    int i,j;
    int coord[2];
    char nomfitxer[250];
    int maxcorrlen=50;
    if (L<maxcorrlen)
        maxcorrlen=L;
    
    DoubleVector mitjcorrel=CreateDoubleVector(maxcorrlen);
    DoubleVector mitjcorrelara=CreateDoubleVector(maxcorrlen);
    DoubleVector acorrelar=CreateDoubleVector(L);
    for (i=0;i<maxcorrlen;++i)
    {
        mitjcorrel[i]=0.0;
    }

    for (j=0;j<L;++j) 
    {
        coord[0]=j;
        for (i=0;i<L;++i)
        {
            coord[1]=i;
            acorrelar[i]=spins[xyz2i(d,L,coord)];
        }
        autocorrel(acorrelar,mitjcorrelara);
        for (i=0;i<maxcorrlen;++i)
        {
            mitjcorrel[i]+=mitjcorrelara[i];
        }
    }
    
    for (j=0;j<maxcorrlen;++j)
        mitjcorrel[j]/=(double)L;
    
    
    makedir("rmig_corr");
    sprintf(nomfitxer,"rmig_corr/correlR%d.txt",iter);
    
    fitx=Fopen(nomfitxer,"w");
    
    for (i=0;i<maxcorrlen;++i)
    {
        fprintf(fitx,"%lf\n",mitjcorrel[i]);
    }
    Fclose(fitx);
    DestroyDoubleVector(acorrelar);
    DestroyDoubleVector(mitjcorrelara);
    DestroyDoubleVector(mitjcorrel);
    return;
}

int GuardaEstat(int iter, IntVector spins, int L, int d)
{
    if (d!=2)
        return 0;
    FILE *fitx;
    int i,j,index;
    int coord[2];
    char nomfitxer[250];
    makedir("estats");
    sprintf(nomfitxer,"estats/estat%d.txt",iter);
    fitx=Fopen(nomfitxer,"w");
    for (i=0;i<L;++i)
    {
        coord[0]=i;
        for (j=0;j<L;++j)
        {
            coord[1]=j;
            index=xyz2i(d,L,coord);
            fprintf(fitx,"%+d ",spins[index]);
        }
        fprintf(fitx,"\n");
    }
    Fclose(fitx);
    return 0;
}


int main(int argc, char **argv)
{
    int d=2,L=20,nmeas=1; //Dimension of the lattice, size of one side, and Nmeas by default
    unsigned long int nmcs=10000000; //Number of MonteCarlo Samples by default
    double T=2.0; //Temperature by default
    int N; //Number of spins
    unsigned long int seed=107; //Seed by default
    int ieq=1000; //Number of MCS, by default, that we ignore by belonging to the termalization
    int E,M; //Energy and magnetization
    double R=-1.0;
    int i;
    int printstate=0; // Whether or not to print the state
    int TypeDyn=0; //0- Glauber, 1- Glauber sequential, 2- Kawasaki, 3- Wolff
    const char *Dynamics[]= {"Glauber single", "Glauber sequential", "Kawasaki (random pairs)", "Wolff", "Kawasaki (neighbour pairs)"};
    
    parseinput(argc,argv,&d,&L,&T,&nmeas,&nmcs,&seed,&ieq,&TypeDyn, &printstate); //Function that read the inputs
    printf("# d: %d\t L : %d\t nmeas: %d \t ieq: %d \t nmcs: %ld\t T= %f\t Algorithm= %s\n",d,L,nmeas,ieq,nmcs,T,Dynamics[TypeDyn]); //Print the inputs
    N=pow(L,d); //Compute N for a square lattice
    InitRNG(seed);
    IntVector spins=CreateIntVector(N); //Vector of spins
    ListIntVector veins = CreateListIntVector(N); //Vector of neighbors
    IntVector sequential; //Sequence of spins for the Glauber sequential updating
    ListIntVector bonds; //Bonds between spins for Wolff algorithm
    InitHypercubicLattice(d,L,veins); // Initialize the geometry.
    if (TypeDyn == 1) // For Glauber dynamics sequential updating
    {
        sequential=CreateIntVector(spins[-1]);
        InitSequential(sequential); //Initialize the sequence randomly (only one sequence per simulation)
    }
    else if (TypeDyn == 3) //For Wolff
    {
        bonds=CreateListIntVector(N);
        InitBonds(N,veins,bonds); //Create the bonds 
    }
    RandomVector(spins); // Random initialization of spins
    E=Energy(spins,veins); // Initial value of energy
    M=Magnetization(spins); // Initial value of magnetization
    //Initialization of exponential table:
    if (TypeDyn != 3)
        Initmyexp(N,veins,T, TypeDyn);
    E+=Metropolis(spins,veins,sequential,bonds,T,ieq,&M,TypeDyn); //Metropolis for the first "ieq" that we ignore
    printf("# E \t M \t R\n");
    for (i=0;i<nmcs/nmeas;++i)
    {
        E+=Metropolis(spins,veins,sequential,bonds,T,nmeas,&M,TypeDyn); //Metropolis for "nmcs" MCS taking measures each "nmeas"
        R=ClusterRadius(spins,d,L);
        printf("%d\t%d\t%lf\n",E,M,R);
        if (printstate==1 && log10(i+1) == round(log10(i+1)))
        {
            GuardaEstat(i+1,spins,L,d);
            CorrelRadi(i+1,spins,L,d);
        }
    }
    if (TypeDyn ==1)
        DestroyIntVector(sequential);
    else if (TypeDyn == 3)
        DestroyListIntVector(bonds,N);
    DestroyIntVector(spins);
    DestroyListIntVector(veins,N);
    if (TypeDyn != 3)
        Destroymyexp();
    FreeRNG();
    return 0;
}

