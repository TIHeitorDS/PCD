#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void read_n(int* n, int my_rank, int comm_sz, MPI_Comm comm);
void allocate_vector(double** vector, double** sum_p, int n);

int main(void) {
    int n;

    double* vector;
    double* sum_p;

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    read_n(&n, my_rank, comm_sz, MPI_COMM_WORLD);
    allocate_vector(&vector, &sum_p, n);

    srand(time(NULL) + my_rank);
    for (int i = 0; i < n; i++){
        vector[i] = rand() % 100;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scan(vector, sum_p, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    printf("%lf ", sum_p[0]);

    free(vector);
    free(sum_p);

    MPI_Finalize();
    
    return 0;
}

void read_n(int* n, int my_rank, int comm_sz, MPI_Comm comm) {
    if (my_rank == 0) {
        printf("Informe o tamanho do vetor:\n");
        scanf("%d", n);
    }

    MPI_Bcast(n, 1, MPI_INT, 0, comm);
}

void allocate_vector(double** vector, double** sum_p, int n) {
    *vector = malloc(n * sizeof(double));
    *sum_p = malloc(n * sizeof(double));
}
