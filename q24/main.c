#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int thread_count;
    int n;
    int *a;

    if (argc != 3) {
        printf("precisa de 2 argumentos");
        exit(1);
    }

    thread_count = strtol(argv[2], NULL, 10);
    n = strtol(argv[1], NULL, 10);
    a = malloc(n*sizeof(int));

    a[0] = 0;

#   pragma omp parallel for num_threads(thread_count)
    for (int i = 1; i < n; i++) {
        a[i] =(i*(i+1))/2;

    }

    for (int i = 0; i < n; i++) {
        printf("%d\t", a[i]);
    }

    printf("\n");
    return 0;
}