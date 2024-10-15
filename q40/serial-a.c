#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    double* vector;
    double* sum_p;
    double sum = 0.0;
    int i;

    printf("informe o tamanho do vetor:\n");
    scanf("%d", &n);

    vector = malloc(n*sizeof(double));
    sum_p = malloc(n*sizeof(double));

    printf("informe os valores para o vetor:\n");
    for (i = 0; i < n; i++) {
        scanf("%lf", &vector[i]);
    }

    for (i = 0; i < n; i++) {
        sum += vector[i];

        sum_p[i] = sum;
    }

    printf("soma parciais: ");
    for (i = 0; i < n; i++) {
       printf("%lf ", sum_p[i]);
    }

    free(vector);

    return 0;
}