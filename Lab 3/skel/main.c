#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_THREADS 4

// scriere in fisier
void write_file(char *filename, char *type, double time, long sum) {
    FILE *file = fopen(filename, "w+");
    fprintf(file, "Type: %s\nTime: %lf\nSum: %ld", type, time, sum);
    fclose(file);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    int size, i;
    int *numbers;

    long sum1 = 0, sum2 = 0, sum3 = 0;
    double t1, t2;
    double time1, time2, time3;
    omp_set_num_threads(NO_THREADS);

    // de paralelizat toata bucata de mai jos

    // fiti atenti la partea de citire din fisier
    fscanf(file, "%d", &size);
    numbers = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        fscanf(file, "%d", &numbers[i]);
    }
    fclose(file);

    // de paralelizat in 3 moduri - atomic, critical si reduction, de masurat timpii de executie
    #pragma omp parallel
    {
        #pragma omp master
        {
            t1 = omp_get_wtime();
        }

        #pragma omp barrier

        #pragma omp for reduction(+:sum1)
        for (i = 0; i < size; i++) {
            sum1 += numbers[i];
        }

        #pragma omp master
        {
            t2 = omp_get_wtime();
            time1 = t2-t1;
            printf("%d\n", sum1);
            sum1 = 0;
            t1 = omp_get_wtime();
        }

        #pragma omp barrier

        #pragma omp for
        for (i = 0; i < size; i++) {
            #pragma omp atomic
            sum1 += numbers[i];
        }

        #pragma omp master
        {
            t2 = omp_get_wtime();
            time2 = t2-t1;
            printf("%d\n", sum1);
            sum1 = 0;
            t1 = omp_get_wtime();
        }
    
        #pragma omp barrier

        #pragma omp for
        for (i = 0; i < size; i++) {
            #pragma omp critical
            sum1 += numbers[i];
        }

        #pragma omp master
        {
            t2 = omp_get_wtime();
            time3 = t2-t1;
            printf("%d\n", sum1);
        }
    }

    // TODO: de scris timpii de executie in 3 fisiere folosind sections (fiecare scriere intr-un section)
    #pragma omp sections
    {
        #pragma omp section
        {
            file = fopen("reductions_time.txt", "w");
            fprintf(file, "%0.6lf\n", time1);
            fclose(file);

            file = fopen("atomic_time.txt", "w");
            fprintf(file, "%0.6lf\n", time2);
            fclose(file);

            file = fopen("critical_time.txt", "w");
            fprintf(file, "%0.6lf\n", time3);
            fclose(file);
        }
    }

    return 0;
}