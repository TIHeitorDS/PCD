#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm);
void allocate_vector(double** local_vector, double** sum_p_local, int local_n);
void read_vector(double local_vector[], int n, int local_n, int my_rank, MPI_Comm comm);
int isPowerOfTwo(int n);

int main(void) {
    int n;
    double local_sum = 0.0;

    double* local_vector;
    double* sum_p_local;
    double* global_sums = NULL; // Buffer de recepção no root
    int local_n;

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    read_n(&n, &local_n, my_rank, comm_sz, MPI_COMM_WORLD);
    allocate_vector(&local_vector, &sum_p_local, local_n);
    read_vector(local_vector, n, local_n, my_rank, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        local_sum += local_vector[i];
        sum_p_local[i] = local_sum;
    }

    if (my_rank != 0) {
        double prev_sum = 0.0;

        MPI_Recv(&prev_sum, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < local_n; i++) {
            sum_p_local[i] += prev_sum;
        }

        local_sum += prev_sum;
    } 

    if (my_rank != comm_sz - 1) {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        global_sums = malloc(n * sizeof(double)); // Tamanho do vetor global
    }

    MPI_Gather(sum_p_local, local_n, MPI_DOUBLE, global_sums, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("Somas prefixadas globais:\n");
        for (int i = 0; i < n; i++) {
            printf("%lf ", global_sums[i]);
        }
        printf("\n");
        free(global_sums);
    }

    free(local_vector);
    free(sum_p_local);

    MPI_Finalize();

    return 0;
}

void read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm) {
    if (my_rank == 0) {
        printf("Informe o tamanho do vetor:\n");
        scanf("%d", n);

        if (!isPowerOfTwo(*n)) {
            printf("n deve ser potencia de 2!\n");
            exit(1);
        }
    }

    MPI_Bcast(n, 1, MPI_INT, 0, comm);

    *local_n = *n / comm_sz;
}

void allocate_vector(double** local_vector, double** sum_p_local, int local_n) {
    *local_vector = malloc(local_n * sizeof(double));
    *sum_p_local = malloc(local_n * sizeof(double));
}

void read_vector(double local_vector[], int n, int local_n, int my_rank, MPI_Comm comm) {
    double* a = NULL;
    int i;

    if (my_rank == 0) {
        a = malloc(n * sizeof(double));

        printf("Informe os valores do vetor:\n");
        for (i = 0; i < n; i++) {
            scanf("%lf", &a[i]);
        }

        MPI_Scatter(a, local_n, MPI_DOUBLE, local_vector, local_n, MPI_DOUBLE, 0, comm);

        free(a);
    } else {
        MPI_Scatter(NULL, local_n, MPI_DOUBLE, local_vector, local_n, MPI_DOUBLE, 0, comm);
    }
}

int isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

