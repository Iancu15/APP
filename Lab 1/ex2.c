#include <stdio.h>
#include <omp.h>
#define N 10000000
int a[N];
int main(void) {

    int i;
    for (i=0;i<N;i++)
        a[i] = 1;

    int sum = 0;
    double t1, t2;
    int tcount = omp_get_num_threads();
    #pragma omp parallel reduction( +:sum )
    {
        int tid = omp_get_thread_num();
        t1 = omp_get_wtime();

        #pragma omp for
        for (i=tid;i<N;i+=tcount)
            sum += a[i];

        t2 = omp_get_wtime();
    }
    
    printf("Sum=%ld, duration=%g\n",sum,t2-t1);
    return 0;
}
