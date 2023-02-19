#include "utils.h"
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 6

typedef struct arg {
    float **matrix;
    int size;
    int id;
    pthread_barrier_t *barrier;
    float *x;
} ThreadArg;

int min(int a, int b) {
    if (a < b)
        return a;
    
    return b;
}

int calculate_start(int thread_id, int dim, int num_of_threads) {
	return thread_id * ceil((double) dim / num_of_threads);
}

int calculate_end(int thread_id, int dim, int num_of_threads) {
	return min((thread_id + 1) * ceil((double) dim / num_of_threads), dim);
}

void gauss_jordan(float **a, size_t n, int start, int end, pthread_barrier_t *barrier)
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

        for (j = start; j < end; j++)
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

        pthread_barrier_wait(barrier);
    }
}

float* calculate_solution(float **a, float *x, size_t n, int start, int end)
{
    int i;
    for (i = start; i < end; i++)
    {
        x[i] = a[i][n + 1] / a[i][i];
    }

    return x;
}

void *thread_function(void *arg) {
    ThreadArg *thread_arg;
    float **matrix, *x;
    pthread_barrier_t *barrier;
    int id, start, end, size;

    thread_arg = arg;
    id = thread_arg->id;
    matrix = thread_arg->matrix;
    size = thread_arg->size;
    barrier = thread_arg->barrier;
    x = thread_arg->x;

    start = calculate_start(id, size, NUM_THREADS) + 1;
    end = calculate_end(id, size, NUM_THREADS) + 1;
    gauss_jordan(matrix, size, start, end, barrier);
    calculate_solution(matrix, x, size, start, end);
}

void main(int argc, char *argv[])
{
    FILE *fp;
    char *line = NULL, *token;
    size_t len = 0;
    size_t size;
    float **matrix, *x;
    int i, j;
    double time;
    pthread_t threads[NUM_THREADS];
    int id;
    ThreadArg arguments[NUM_THREADS];
    pthread_barrier_t *barrier;
    int r;
    void *status;

    Input *input = read_input();
    time = get_time();
    
    x = malloc(sizeof(float) * (input->size + 1));
    barrier = malloc(sizeof(pthread_barrier_t));
    r = pthread_barrier_init(barrier, NULL, NUM_THREADS);
    if (r < 0) {
        printf("Eroare initializare bariera\n");
        exit(EXIT_FAILURE);
    }

    for (id = 0; id < NUM_THREADS; id++) {
        arguments[id].id = id;
        arguments[id].matrix = input->matrix;
        arguments[id].size = input->size;
        arguments[id].barrier = barrier;
        arguments[id].x = x;
        r = pthread_create(&threads[id], NULL, thread_function, (void *) &arguments[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %d\n", id);
            exit(-1);
        }
    }

    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);

        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", id);
            exit(-1);
        }
    }

    r = pthread_barrier_destroy(barrier);
    if (r < 0) {
        printf("Eroare dezalocare bariera\n");
        exit(EXIT_FAILURE);
    }

    printf("time taken %.6lf\n", get_time() - time);
    //display_solution(x, input->size);
    free_matrix(input->matrix, input->size);
    free(input);
}