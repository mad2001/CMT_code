/************************* includes **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>

/************************* defines ***********************************/
#define SIZE 10
#define KISS_MAX 4294967296.0

static unsigned int CNG,    /* variable for congruential generator */
                    XSH,    /* variable for xor shifts */
                    MWC,    /* variable for multiply-with-carry */
                    c;      /* variable to perform the MWC */

/******************* forward declarations ****************************/
unsigned int devrand(void);
void init_KISS();
unsigned int KISS();
double random_number();
int initialize( int lat[ ] );
int local_energy( int lat[ ], int );
void flip(int lat[ ], int );
int total_energy( int lat[ ] );
int total_magnetization( int lat[ ] );


int main()
{
    FILE *fp;
    int lattice[ SIZE+1 ];      /* 1d lattice for spins */
    double minT = 0.5;          /* minimum temperature */
    double maxT = 5.0;          /* maximum temperature */
    double T;                   /* temperature loop variable */
    double change = 0.1;        /* step size for temperature loop */
    int steps = 100;            /* number of Monte Carlo steps */
    int i, j;                   /* loop variables */
    double norm = ( 1 / (double)(steps * SIZE) );

    double E = 0, E_avg, E_tot;        /* for energy observables */
    double M = 0, M_avg, M_tot;        /* for magnetization observables */
    int de;
    int flips = 0;

    init_KISS();      /* initialize random number generator */

    fp = fopen("data.txt", "w");
    fprintf(fp, "# temperature, energy, magnetization\n");


    /* temperature loop */
    for (T=minT; T<=maxT; T+=change)
    {
        initialize(lattice);
        E_tot = 0;
        M_tot = 0;

        /* Monte Carlo loop */
        for (i=0; i<=steps; i++)
        {
            E = total_energy(lattice);
            M = total_magnetization(lattice);

            /* Metropolis loop */
            for (j=0; j<=SIZE; j++)
            {
                de = -2 * local_energy(lattice, j);
                if ( ( de < 0 ) )
                {
                    flip(lattice, j);
                    E += 2 * de;
                    M += 2 * lattice[j];
                    flips +=1;
                }
                else
                    if ( random_number() < exp(-de) )
                    {
                        flip(lattice, j);
                        E += 2 * de;
                        M += 2 * lattice[j];
                        flips +=1;
                    }
            }

            E_tot += E;
            M_tot += M;
        }
        E_avg = E_tot * norm;
        M_avg = M_tot * norm;
        /* printf("Average energy is %f and average magnetization is %f.\n", E_avg, M_avg); */
        fprintf(fp, "%f, %f, %f\n", T, E_avg, M_avg);
    }
    fclose(fp);

    return 0;
}



/* returns random 32-bit integers */
unsigned int devrand(void)
{
    int fn;
    unsigned int r;
    fn = open("/dev/urandom", O_RDONLY);
    if (fn == -1)
        exit(-1); /* Failed! */
    if (read(fn, &r, 4) != 4)
       exit(-1); /* Failed! */
    close(fn);
    return r;
}


/* initialise KISS generator with random SEED */
void init_KISS()
{
    CNG = devrand();
    while ( !(XSH = devrand()) )
        ; /* XSH must not be zero! */
    MWC = devrand();

    /* We don’t really need to set c as well but let's anyway... */
    /* NOTE: offset c by 1 to avoid MWC=c=0 */
    c = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}


/* returns radom number from KISS RNG */
unsigned int KISS()
{
   unsigned long long t;

   /* linear congruential generator */
   CNG = 314527869 * CNG + 1234567;

   /* three xor shifts */
   XSH ^= (XSH << 5); XSH ^= (XSH >> 7); XSH ^= (XSH << 22);

   /* multipy-with-carry */
   t = 4294584393ULL * MWC + c; c = (t >> 32); MWC = t;

   return CNG + XSH + MWC;
}



/*********************************************************************
 *  random_number generates a random number between 0 and 1
 *********************************************************************/
double random_number()
{
    return  KISS() / 4294967296.0;
}


/*********************************************************************
 *  initialize will randomly fill the lattice
 *********************************************************************/
int initialize( int lat[ SIZE+1 ] )
{
    int i;
    for (i=0; i<=SIZE; i++)
    {
        if (random_number()>=0.5)
            lat[i]=1;
        else
            lat[i]=-1;
    }
    return 0;
}


/*********************************************************************
 *  local_energy calculates the energy at a specified spin site
 *********************************************************************/
int local_energy( int lat[ SIZE+1 ], int pos )
{
    int left, right;

    if ( pos == 0 )
        left = 1;
    else
        left = lat[pos-1];
    if ( pos == SIZE )
        right = 1;
    else
        right = lat[pos+1];

    return -1 * ( (lat[pos] * right) + (lat[pos] * left) );
}


/*********************************************************************
 *  flip changes the spin at a given location
 *********************************************************************/
void flip(int lat[ SIZE+1 ], int pos)
{
    lat[pos] = -lat[pos];
}


/*********************************************************************
 *  total_energy calculates the total energy of the lattice
 *********************************************************************/
int total_energy( int lat[ SIZE+1 ] )
{
    int i, e = 0;
    for (i=0; i<=SIZE; i++)
    {
        e += local_energy(lat, i);
    }
    return e;
}


/*********************************************************************
 *  total_magnetization calculates the total magnetization of the lattice
 *********************************************************************/
int total_magnetization( int lat[ SIZE+1 ] )
{
    int i, m = 0;
    for (i=0; i<=SIZE; i++)
    {
        m += lat[i];
    }
    return m;
}
