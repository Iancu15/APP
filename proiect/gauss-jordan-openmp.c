#include "utils.h"
#include <omp.h>

void gauss_jordan(float **a, size_t n)
{
    int i, j, k;
    float ratio;

    for (i = 1; i <= n; i++)
    {
        if (a[i][i] == 0.0)
        {
            printf("Mathematical Error!");
            exit(EXIT_FAILURE);
        }

        for (j = 1; j <= n; j++)
        {
            if (i != j)
            {
                ratio = a[j][i] / a[i][i];
#pragma omp parallel for private(k) shared(a)
                for (k = 1; k <= n + 1; k++)
                {
                    a[j][k] = a[j][k] - ratio * a[i][k];
                }
            }
        }
    }
}

float* calculate_solution(float **a, size_t n)
{
    int i;
    float *x;
    x = malloc(sizeof(float) * (n + 1));
#pragma omp parallel for private(i) shared(x)
    for (i = 1; i <= n; i++)
    {
        x[i] = a[i][n + 1] / a[i][i];
    }

    return x;
}

void main(int argc, char *argv[])
{
    FILE *fp;
    char *line = NULL, *token;
    size_t len = 0;
    size_t size;
    float **matrix, *x;
    int i, j, number_of_threads;
    double time;

    if (argc != 2)
    {
        printf("There's only one argument and that is the number of threads!\n");
        exit(EXIT_FAILURE);
    }

    number_of_threads = atoi(argv[1]);
    if (number_of_threads <= 0)
    {
        printf("Invalid number of threads!\n");
        exit(EXIT_FAILURE);
    }

    omp_set_num_threads(number_of_threads);
    Input *input = read_input();
    time = get_time();
    gauss_jordan(input->matrix, input->size);
    x = calculate_solution(input->matrix, input->size);
    printf("time taken %.6lf\n", get_time() - time);
    //display_solution(x, input->size);
    free_matrix(input->matrix, input->size);
    free(input);
}