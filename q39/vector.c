#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm);
void read_k(int* k, int my_rank, MPI_Comm comm);
void allocate_vectors(double** local_vector1, double** local_vector2, double** local_vector_prod, int local_n);
void read_vector(double local_a[], int local_n, int n, int my_rank, MPI_Comm comm);
void vector_k(double local_vector1[], int k, int local_n);
void vectors_dot(double local_vector1[], double local_vector2[], double local_vector_prod[], int local_n);
void print_vector(double* local_vector_prod, int n, int local_n, int my_rank, MPI_Comm comm);

int main(void) {
    double* local_vector1;
    double* local_vector2;
    double* local_vector_prod;
    int k;

    int n;
    int local_n;
    int my_rank;
    int comm_sz;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    read_n(&n, &local_n, my_rank, comm_sz, comm);
    read_k(&k, my_rank, comm);
    allocate_vectors(&local_vector1, &local_vector2, &local_vector_prod, local_n);
    read_vector(local_vector1, local_n, n, my_rank, MPI_COMM_WORLD);
    read_vector(local_vector2, local_n, n, my_rank, MPI_COMM_WORLD);
    vector_k(local_vector1, k, local_n);
    vectors_dot(local_vector1, local_vector2, local_vector_prod, local_n);
    print_vector(local_vector_prod, n, local_n, my_rank, MPI_COMM_WORLD);


    free(local_vector1);
    free(local_vector2);
    free(local_vector_prod);

    MPI_Finalize();

    return 0;
}

void read_n(int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm) {
    if (my_rank == 0) {
        printf("informe o tamanho do vetor: \n");
        scanf("%d", n);
    }

    MPI_Bcast(n, 1, MPI_INT, 0, comm);
    *local_n = *n/comm_sz;
}

void read_k(int* k, int my_rank, MPI_Comm comm) {
    if (my_rank == 0) {
        printf("informe o valor da constante: \n");
        scanf("%d", k);
    }

    MPI_Bcast(k, 1, MPI_INT, 0, comm);
}

void allocate_vectors(double** local_vector1, double** local_vector2, double** local_vector_prod, int local_n) {
    *local_vector1 = malloc(local_n*sizeof(double));
    *local_vector2 = malloc(local_n*sizeof(double));
    *local_vector_prod = malloc(local_n*sizeof(double));
}

void read_vector(double local_a[], int local_n, int n, int my_rank, MPI_Comm comm) {
    double* a = NULL;
    int i;

    if (my_rank == 0) {
        a = malloc(n*sizeof(double));

        printf("digite os valores para o vetor: \n");
        for(i = 0; i < n; i++) 
            scanf("%lf", &a[i]);
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
        free(a);
    } else {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    }

}

void vector_k(double local_vector1[], int k, int local_n) {
    int i;
    
    for (i = 0; i < local_n; i++) {
        local_vector1[i] = local_vector1[i] * k;
    }
}

void vectors_dot(double local_vector1[], double local_vector2[], double local_vector_prod[], int local_n) {
    int i;

    for (i = 0; i < local_n; i++) {
        local_vector_prod[i] = local_vector1[i] * local_vector2[i];
    }
}

void print_vector(double* local_b, int n, int local_n, int my_rank, MPI_Comm comm) {
    int i;
    double* b = NULL;

    if (my_rank == 0) {
        b = malloc(n * sizeof(double));

        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);

        printf("produto interno do vetor: \n");
        for (i = 0; i < n; i++) {
            printf("%f ", b[i]);
        }
        printf("\n");

        free(b);
    } else {
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
    }
}
