#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "functions_2d.h"
#include "../random_numbers/kiss_rng.h"


/* randomly fill the lattice */
void fill_lattice( int lat[ SIZE+1 ][ SIZE+1 ] )
{
    int i, j;
    for (i=0; i<=SIZE; i++)
    {
        for (j=0; j<=SIZE; j++)
        {
            if ( random_number()>=0.5 )
                lat[i][j] = 1;
            else
                lat[i][j] = -1;
        }
    }
    return;
}


/* calculate the energy at a specified spin site */
int local_energy( int lat[ SIZE+1 ][ SIZE+1 ], int row, int col )
{
    int left, right, up, down;

    if ( row == 0 )
        up = lat[SIZE][col];
    else
        up = lat[row-1][col];
    if ( row == SIZE )
        down = lat[0][col];
    else
        down = lat[row+1][col];
    if ( col == 0 )
        left = lat[row][SIZE];
    else
        left = lat[row][col-1];
    if ( col == SIZE )
        right = lat[row][0];
    else
        right = lat[row][col+1];

    return -1 * J * lat[row][col] * (right+left+up+down) - h*lat[pos];
}


/* calculate local energy after flipping */
int new_energy(int lat[ SIZE+1 ], int pos)
{
    int left, right, s;

    s = -lat[pos];
    /* periodic boundary conditions */
    if ( row == 0 )
        up = lat[SIZE][col];
    else
        up = lat[row-1][col];
    if ( row == SIZE )
        down = lat[0][col];
    else
        down = lat[row+1][col];
    if ( col == 0 )
        left = lat[row][SIZE];
    else
        left = lat[row][col-1];
    if ( col == SIZE )
        right = lat[row][0];
    else
        right = lat[row][col+1];

    return -1 * J * s * (right + left) - h*s;
}


/* decide whether or not to flip the spin */
int test_flip( int lat[ SIZE+1 ], int pos, double T, int de )
{
    if ( de < 0 )
    {
        return 1;
    }
    else
        if ( random_number() < exp(-de/(K_B * T)) )
            return 1;
        else
            return 0;
}



/* flip the spin at a given location */
void flip( int lat[ SIZE+1 ], int pos )
{
    lat[pos] = -lat[pos];
}


/* run through Monte Carol loop to initialize/randomize */
void initialize( int lat[ SIZE+1 ], double T)
{
    int de, i, j, steps = 10000;

    /* Monte Carlo loop */
    for (i=0; i<=steps; i++)
    {
        /* Metropolis loop */
        for (j=0; j<=SIZE; j++)
        {
            de = new_energy(lat, j) - local_energy(lat, j);
            if ( test_flip(lat, j, T, de) )
            {
                flip(lat, j);
            }
        }
    }
    return;
}


/* calculate the total energy of the lattice */
int total_energy( int lat[ SIZE+1 ] )
// @TODO is this actually adding energies correctly? are we double counting?
{
    int i, e = 0;
    for (i=0; i<=SIZE; i++)
    {
        e += local_energy(lat, i);
    }
    return e;
}


/* calculate the total magnetization of the lattice */
int total_magnetization( int lat[ SIZE+1 ] )
{
    int i, j, m = 0;
    for (i=0; i<=SIZE; i++)
    {
        for (j=0; j<=SIZE; j++)
        {
            m += lat[i];
        }
    }
    return m;
}
