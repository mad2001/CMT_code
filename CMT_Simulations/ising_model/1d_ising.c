/************************* includes **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "MC_functions.h"
#include "random_numbers/kiss_rng.h"


/************************ begin main **********************************/
int main()
{
    FILE *fp;                   /* file to output data */
    int lattice[ SIZE+1 ];      /* 1d lattice for spins */
    double minT = 0;            /* minimum temperature */
    double maxT = 5.0;          /* maximum temperature */
    double T;                   /* temperature loop variable */
    double change = 0.1;        /* step size for temperature loop */
    int steps = 10000;          /* number of Monte Carlo steps */
    int i, j;                   /* loop variables */

    double E = 0, E_avg, E_tot, Esq_tot, Esq_avg;        /* for energy observables */
    double M = 0, M_avg, M_tot;        /* for magnetization observables */
    double C_v;                        /* specific heat */
    double norm;
    int de;
    int samples;


    /* initialize random number generator */
    init_KISS();

    fp = fopen("1d_data.txt", "w");
    fprintf(fp, "# temperature, energy, magnetization, specific heat\n");

    fill_lattice(lattice);

    /* temperature loop */
    for (T=maxT; T>=minT; T-=change)
    {
        initialize(lattice, T);

        E = total_energy(lattice);
        M = total_magnetization(lattice);

        E_tot = 0;
        Esq_tot = 0;
        M_tot = 0;
        samples = 0;

        /* Monte Carlo loop */
        for (i=0; i<=steps; i++)
        {
            /* Metropolis loop */
            for (j=0; j<=SIZE; j++)
            {
                de = new_energy(lattice, j) - local_energy(lattice, j);
                if ( test_flip(lattice, j, T, de) )
                {
                    flip(lattice, j);
                    E += de;
                    M += 2 * lattice[j];
                }
            }
            if ( i % 20 == 0 )
            {
                samples++;
                E_tot += E / 2.0;
                Esq_tot += (E / 2.0) * (E / 2.0);
                M_tot += M;
            }
        }
        if ( T<=3 )
        {
            norm = 1 / ((double)samples * SIZE);
            E_avg = E_tot * norm;
            Esq_avg = (Esq_tot * norm) / SIZE;
            M_avg = M_tot * norm;

            C_v = (1 / (T*T)) * (Esq_avg - E_avg*E_avg);
            fprintf(fp, "%f, %f, %f, %f\n", T, E_avg, M_avg, C_v);
            printf("%f, %f, %f\n", (1 / (T*T)), Esq_avg, E_avg*E_avg);
        }
    }
    fclose(fp);

    return 0;
}
/************************* end main **********************************/
