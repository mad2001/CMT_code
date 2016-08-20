
#include <stdio.h>

/************************* defines ***********************************/
#define SIZE 100                         /* lattice size */
#define K_B 1                            /* Boltzmann costant */
#define J 1                              /* energy constant */
#define h 1                             /* coupling constant */


void fill_lattice( int lat[ ] );
int local_energy( int lat[ ], int );
int new_energy(int lat[ ], int );
int test_flip( int lat[ ], int, double, int );
void flip(int lat[ ], int );
int total_energy( int lat[ ] );
int total_magnetization( int lat[ ] );
