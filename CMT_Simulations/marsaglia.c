/* Public domain code for JKISS RNG
    period of about 2^127*/
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define KISS_MAX 4294967296.0

static unsigned int CNG,    /* variable for congruential generator */
                    XSH,    /* variable for xor shifts */
                    MWC,    /* variable for multiply-with-carry */
                    c;      /* variable to perform the MWC */


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


int main()
{
    int i;
    init_KISS();
    for (i=0; i<30; i++)
        printf("%f\n", KISS() / 4294967296.0);

    return 0;
}
