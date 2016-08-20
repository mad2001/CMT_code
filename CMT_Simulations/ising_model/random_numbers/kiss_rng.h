#include <stdio.h>
#include <stdint.h>

/************************* defines ***********************************/
#define KISS_MAX 4294967296.0            /* maximum RNG size */

static unsigned int CNG,    /* variable for congruential generator */
                    XSH,    /* variable for xor shifts */
                    MWC,    /* variable for multiply-with-carry */
                    c;      /* variable to perform the MWC */

unsigned int devrand(void);
void init_KISS();
unsigned int KISS();
void initialize( int lat[ ], double);
double random_number();
