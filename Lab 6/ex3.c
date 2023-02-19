#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SENDER 0
#define RECEIVER 1

#define ROOT 0

const int chunks_per_process = 5; // numarul de elemente per proces

int* create_array (int size) {
    int* arr = (int*) malloc (size * sizeof(int));
    
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }

    return arr;
}

int* increaseEveryElement (int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i]++;
    }

    return arr;
}

// aflam maximum dintr-un array
int sum_array (int* arr, int size) {
    int sum = 0;

    for (int i = 0; i < size; ++i) {
	sum += arr[i];
    }

    return sum;
}


int main (int argc, char **argv) {
    // rank = id-ul procesului
    // proc = numarul de procese
    int rank, proc, *arr;
    int sum = 0;
    int coeff;
    // initializam procesele
    MPI_Init(&argc, &argv);
    
    // fiecare proces are un rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // rank
    // stabilim numarul de procese
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    // send si receive
    // un proces ii trimite informatia altui proces
    if (rank == SENDER) {
        arr = create_array(chunks_per_process * (proc - 1));
      	//printf("proc = %d", proc);

	for (int i = 1; i < proc; i++ ) {
		MPI_Send(arr + (i - 1) * chunks_per_process, chunks_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	//scanf("PAM PAM: %d", &coeff);
	coeff = 2;
	// printf("Value %d was sent by the process no. %d\n", arr, rank);
    }

    MPI_Bcast(&coeff, 1, MPI_INT, ROOT, MPI_COMM_WORLD);


    if (rank != SENDER) {
        MPI_Recv(arr, chunks_per_process, MPI_INT, SENDER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	int s = sum_array(arr, chunks_per_process) * coeff;
	
	MPI_Send(&s, 1, MPI_INT, SENDER, 0, MPI_COMM_WORLD);
    }

	
    if (rank == SENDER) {
	int s;
    	for (int i = 1; i < proc; i++ ) {
		MPI_Recv(&s, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum += s;
	}

	printf("SUMA FINALA ESTE: %d\n", sum);
	printf("Vectorul a fost: ");
	for (int i = 0; i < chunks_per_process * (proc - 1); i++ ) {
		printf("%d ", arr[i]);
	}
	printf("\n");
    }

    MPI_Finalize();
    return 0;
}
