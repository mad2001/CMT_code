/************************* includes **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include "kiss_rng.h"
#include "MC_functions.h"

#define TESTS 21

/************************ begin main **********************************/
int main()
{
    FILE *fp;                   /* file to output data */
    int lattice[ SIZE+1 ];      /* 1d lattice for spins */
    double T = 1;                 /* temperature loop variable */
    int i, j, k;                   /* loop variables */

    double E, E_avg, E_tot=0;        /* for energy observables */
    double norm;
    int de;
    int samples;

    int step[TESTS] = {1, 5, 10, 25, 50, 100, 150, 200, 250, 500, 750, 1000,
                        5000, 10000, 50000, 75000, 100000, 200000, 500000, 750000};

    /* initialize random number generator */
    init_KISS();

    fp = fopen("MCsteps.txt", "w");
    fprintf(fp, "# steps, energy\n");

    fill_lattice(lattice);
    initialize(lattice, T);

    E = total_energy(lattice);
    E_tot = 0;
    samples = 0;
    i = 0;
    k = 0;

    while( i <= 18 )
    {
        /* Metropolis loop */
        for (j=0; j<=SIZE; j++)
        {
            de = new_energy(lattice, j) - local_energy(lattice, j);
            if ( test_flip(lattice, j, T, de) )
            {
                flip(lattice, j);
                E += de;
            }
        }

        if ( k % 20 == 0 )
        {
            E_tot += E / 2.0;
            samples++;
        }

        if ( samples == step[i] )
        {
            norm =  1 / ((double)samples * SIZE);
            E_avg = E_tot * norm;
            fprintf(fp, "%d, %f\n", samples, E_avg);
            i++;
        }
        k++;
    }

    fclose(fp);

    return 0;
}
