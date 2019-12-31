#include <stdio.h>
#include <sys/time.h>                /* for gettimeofday() */
#include <stdlib.h>

/* Resolution in Milli-Seconds. NOT THOUROUGHLY TESTED */
int main()
{
    struct timeval t1, t2;
    double elapsedTime;

    /* start timer */
    gettimeofday(&t1, NULL);

    /* do something */
    int i,j=0,k=0;
    for (i=1;i<=10;++i)
    { j = i*i;
      k = i*i*i;
      /* printf("%3d %5d %8d\n",i, j, k); */
     }

    /* stop timer */
    gettimeofday(&t2, NULL);

    /* compute and print the elapsed time in millisec */
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("elapsedTime=%g msec\n", elapsedTime);

    getchar();
    return 0;
}
