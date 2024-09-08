#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Insira três argumentos!");
        exit(1);
    }

    int n;
    int thread_count;

    i = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);

    #pragma omp parallel num_threads(thread_count) {
        int my_rank = omp_get_thread_num();
        printf("Thread %d: Iteracoes ", omp_get_thread_num());

        #pragma omp for
        for (int i = 0; i < n; i++) {
            printf("%d -- %d", i, i + 1);
        }
    }

    return 1;
}