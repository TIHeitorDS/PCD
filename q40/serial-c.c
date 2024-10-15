#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv) {
    int rank, size, n, local_n, i;
    double *vector = NULL, *local_vector = NULL, *sum_p = NULL, *local_sum_p = NULL;
    double local_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Informe o tamanho do vetor (potência de 2):\n");
        scanf("%d", &n);
    }

    // Broadcast do tamanho do vetor
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_n = n / size;

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

    // Distribuição dos dados
    MPI_Scatter(vector, local_n, MPI_DOUBLE, local_vector, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Cada processo calcula sua soma local
    for (i = 0; i < local_n; i++) {
        local_sum += local_vector[i];
        local_sum_p[i] = local_sum;
    }

    int log_p = log2(size);
    double received_sum = 0.0;

    for (int phase = 0; phase < log_p; phase++) {
        int partner = rank ^ (1 << phase);

        if (partner < size) {
            MPI_Sendrecv(&local_sum, 1, MPI_DOUBLE, partner, 0, &received_sum, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (partner < rank) {
                local_sum += received_sum;
            }
        }
    }

    for (i = 0; i < local_n; i++) {
        local_sum_p[i] += (rank > 0 ? received_sum : 0);
    }

    MPI_Gather(local_sum_p, local_n, MPI_DOUBLE, sum_p, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
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
