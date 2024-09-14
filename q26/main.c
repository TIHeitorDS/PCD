#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso correto: %s <número de iterações> <número de threads>\n", argv[0]);
        exit(1);
    }

    int n = strtol(argv[1], NULL, 10);
    int thread_count = strtol(argv[2], NULL, 10);

    double soma_total = 0.0;

    #pragma omp parallel num_threads(thread_count)
    {
        int i;
        double minha_soma = 0.0;

        for (i = 0; i < n; i++) {
            #pragma omp atomic
            minha_soma += sin(i);
        }

        soma_total += minha_soma;
    }

    printf("Resultado da soma: %.2f\n", soma_total);

    return 0;
}
