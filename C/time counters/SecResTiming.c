#include <stdio.h>
#include <stdlib.h>
/* Include the time.h library */
#include <time.h>

/* Resolution Seconds */

int main(void)
{
/* Declare time variables */
   clock_t t1, t2;
   double ratio = 1./CLOCKS_PER_SEC;
   
   /* Record the start time */
   t1 = clock();
   
/*************************
Run a segment of Code
*************************/				 
    int i,j=0,k=0;
    for (i=1;i<=10000;++i)
    { j = i*i;
      k = i*i*i;
      printf("%3d %5d %8d\n",i, j, k); 
     }

/* Record the end time */
   t2 = clock(); 
   
/* Print out the difference, converting it to seconds */
   printf("\nTime = %g seconds", ratio*(long)t2 - ratio*(long)t1 );
   
   getchar();
   return 0;
}
