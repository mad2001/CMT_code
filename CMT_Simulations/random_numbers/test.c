#include <stdio.h>
#include "kiss_rng.h"

int main()
{
    int i;                      /* loop variable */
    int total = 5000;           /* number of test points */


    for (i=0; i<total; i++)
    {
        printf("%f, %f\n", random_number(), random_number());
    }

    return 0;
}
