#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    long long int qnt_no_circulo = 0;
    long long int num_lancamentos;
    double x, y;
    double estimativa_de_pi, distancia_quadrada;
    int qnt_thread;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_lancamentos> <qnt_thread>\n", argv[0]);
        exit(1);
    }

    num_lancamentos = strtol(argv[1], NULL, 10);
    qnt_thread = strtol(argv[2], NULL, 10);

    unsigned int seed = time(NULL);

#   pragma omp parallel num_threads(qnt_thread) private(x, y, distancia_quadrada) reduction(+:qnt_no_circulo)
    {
        unsigned int thread_seed = seed + omp_get_thread_num();
        #pragma omp for
        for (long long int lancamento = 0; lancamento < num_lancamentos; lancamento++) {
            x = (double)rand_r(&thread_seed) / RAND_MAX * 2.0 - 1.0;
            y = (double)rand_r(&thread_seed) / RAND_MAX * 2.0 - 1.0;

            distancia_quadrada = x * x + y * y;

            if (distancia_quadrada <= 1.0) qnt_no_circulo++;
        }
    }

    estimativa_de_pi = 4 * (qnt_no_circulo / (double)num_lancamentos);

    printf("Estimativa de PI: %f\n", estimativa_de_pi);

    return 0;
}