#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size, n, local_n, i;
    double *vector = NULL, *local_vector = NULL, *sum_p = NULL, *local_sum_p = NULL;
    double local_sum = 0.0, prefix_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Informe o tamanho do vetor:\n");
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_n = n / size;
    if (rank == size - 1) {
        local_n += n % size; // O último processo lida com o restante
    }

    if (rank == 0) {
        vector = malloc(n * sizeof(double));
        sum_p = malloc(n * sizeof(double));

        printf("Informe os valores para o vetor:\n");
        for (i = 0; i < n; i++) {
            scanf("%lf", &vector[i]);
        }
    }

    local_vector = malloc(local_n * sizeof(double));
    local_sum_p = malloc(local_n * sizeof(double));

    MPI_Scatter(vector, local_n, MPI_DOUBLE, local_vector, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (i = 0; i < local_n; i++) {
        local_sum += local_vector[i];
        local_sum_p[i] = local_sum;
    }

    if (rank != 0) {
        double previous_sum = 0.0;
        MPI_Recv(&previous_sum, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for (i = 0; i < local_n; i++) {
            local_sum_p[i] += previous_sum;
        }

        local_sum += previous_sum;
    }

    if (rank != size - 1) {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Gather(local_sum_p, local_n, MPI_DOUBLE, sum_p, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Impressão do resultado final
        printf("Somas parciais: ");
        for (i = 0; i < n; i++) {
            printf("%lf ", sum_p[i]);
        }
        printf("\n");

        free(vector);
        free(sum_p);
    }

    free(local_vector);
    free(local_sum_p);

    MPI_Finalize();
    return 0;
}
