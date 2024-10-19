#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(void) {
    int n;
    int count;
    int* matrix = NULL;
    int* matrix_flat = NULL;

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        printf("Informe o tamanho da matriz: ");
        fflush(stdout);
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Datatype triangle_type;

    if (my_rank == 0) {
        int blocklengths[n];
        int displacements[n];

        for (int i = 0; i < n; i++) {
            blocklengths[i] = n - i; // número de elementos em cada bloco
            displacements[i] = (i * n) + i; // deslocamento de cada bloco
        }

        MPI_Type_indexed(n, blocklengths, displacements, MPI_INT, &triangle_type);
        MPI_Type_commit(&triangle_type);

        matrix = malloc(n * n * sizeof(int));

        printf("Informe os elementos da matriz:\n");
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                printf("elemento [%d][%d]: ", i, j);
                fflush(stdout);
                scanf("%d", &matrix[(i * n) + j]);
            }
        }

        MPI_Send(matrix, 1, triangle_type, 1, 0, MPI_COMM_WORLD);
    } else {
        count = n * (n + 1) / 2;
        matrix_flat = malloc(count * sizeof(int));

        MPI_Recv(matrix_flat, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Elementos da parte triangular superior: ");
        fflush(stdout);
        for(int i = 0; i < count; i++) {
            printf("%d ", matrix_flat[i]);
        }
        printf("\n");
    }

    if (my_rank == 0) {
        free(matrix);
    }

    free(matrix_flat);
    
    MPI_Finalize();

    return 0;
}
