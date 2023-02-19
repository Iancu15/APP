#ifndef _UTILS_HEADER
#define _UTILS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double get_time()
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec + now.tv_nsec*1e-9;
}

void print_matrix(float **matrix, size_t size)
{
    int i, j;
    for (i = 1; i <= size; i++)
    {
        for (j = 1; j <= size + 1; j++)
        {
            printf("%f ", matrix[i][j]);
        }

        printf("\n");
    }
}

void display_solution(float *x, size_t n)
{
    int i;
    printf("\nSolution:\n");
    for (i = 1; i <= n; i++)
    {
        printf("x[%d] = %0.3f\n", i, x[i]);
    }
}

void free_matrix(float **matrix, size_t size)
{
    int i;
    for (i = 1; i <= size; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

typedef struct input {
    float **matrix;
    int size;
} Input;

Input *read_input() {
    FILE *fp;
    char *line = NULL, *token;
    size_t len = 0;
    size_t size;
    float **matrix;
    int i, j;

    fp = fopen("input", "r");
    if (!fp)
        exit(EXIT_FAILURE);

    getline(&line, &len, fp);
    token = strtok(line, " ");
    size = atoi(token);
    matrix = malloc((size + 1) * sizeof(float *));
    for (i = 1; i <= size; i++)
    {
        matrix[i] = malloc((size + 2) * sizeof(float));
    }

    i = 1;
    while (getline(&line, &len, fp) != -1)
    {
        token = strtok(line, " ");
        for (j = 1; j <= size + 1; j++)
        {
            matrix[i][j] = atoi(token);
            token = strtok(NULL, " ");
        }

        i++;
    }

    fclose(fp);
    free(line);

    Input *input = malloc(sizeof(Input));
    input->matrix = matrix;
    input->size = size;

    return input;
}

#endif