#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Insira três argumentos!\n");
        exit(1);
    }

    int n;
    int thread_count;

    n = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);

    #pragma omp parallel num_threads(thread_count)
    {
        int my_rank = omp_get_thread_num();
        int start = -1, end = -1;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            if (start == -1) start = i;
            end = i;
        }

        printf("Thread %d: Iterações %d -- %d\n", my_rank, start, end);
    }

    return 0;
}
