#include "utils.h"

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
    for (i = 1; i <= n; i++)
    {
        x[i] = a[i][n + 1] / a[i][i];
    }

    return x;
}

void main()
{
    float *x;
    double time;

    Input *input = read_input();
    time = get_time();
    gauss_jordan(input->matrix, input->size);
    x = calculate_solution(input->matrix, input->size);
    printf("time taken %.6lf\n", get_time() - time);
    //display_solution(x, input->size);
    free_matrix(input->matrix, input->size);
    free(input);
}