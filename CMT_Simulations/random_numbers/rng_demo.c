/************************* includes **********************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>

/************************* defines ***********************************/
#define KISS_MAX 4294967296.0            /* maximum RNG size */

static unsigned int CNG,    /* variable for congruential generator */
                    XSH,    /* variable for xor shifts */
                    MWC,    /* variable for multiply-with-carry */
                    c;      /* variable to perform the MWC */


/******************* forward declarations ****************************/
unsigned int devrand(void);
void init_KISS();
unsigned int KISS();
void initialize( int lat[ ], double);
double random_number();


/************************ begin main **********************************/
int main()
{
    FILE *fp;                   /* file to output data */
    int i;                      /* loop variable */
    int total = 5000;           /* number of test points */

    fp = fopen("random_numbers.txt", "w");

    for (i=0; i<total; i++)
    {
        fprintf(fp, "%f, %f\n", random_number(), random_number());
    }

    fclose(fp);

    return 0;
}
/************************* end main **********************************/



/*********************************************************************
 *            random number generator functions                      *
 *********************************************************************/

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


/* returns random number from KISS RNG */
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


/* random_number generates a random number between 0 and 1 */
double random_number()
{
    return  KISS() / 4294967296.0;
}
