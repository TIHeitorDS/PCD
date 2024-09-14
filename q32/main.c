#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

void Count_sort(int a[], int n);
void Count_sort_parallel(int a[], int n,int thread_count);

int main(int argc, char* argv[]) {
    int a[] = {1, 1, 4, 6, 7, 1, 6, 9, 10, 11, 2};
    int n = 11;

    // if (argc != 1) exit(1);

    int thread_count = strtol(argv[1], NULL, 10);

    Count_sort_parallel(a, n, thread_count);

    for (int i = 0; i < n; i++) {
        printf("%d\t", a[i]);
    }

    printf("\n");

    return 0;
}

void Count_sort(int a[], int n) {
    int i, j, count;
    int* temp = malloc(n*sizeof(int));
        for (i = 0; i < n; i++) {
            count = 0;
            for (j = 0; j < n; j++) {
                if (a[j] < a [i])
                    count++;
                else if (a[j] == a[i] && j < i)
                    count++;
            }
            temp[count] = a[i];
    }

    memcpy(a, temp, n*sizeof(int));
    free(temp);
}

void Count_sort_parallel(int a[], int n, int thread_count) {
    int i, j, count;
    int* temp = malloc(n*sizeof(int));

    # pragma omp parallel for num_threads(thread_count) private(i, j, count) shared(temp, a, n)
        for (i = 0; i < n; i++) {
            count = 0;
            for (j = 0; j < n; j++) {
                if (a[j] < a [i])
                    count++;
                else if (a[j] == a[i] && j < i)
                    count++;
            }
            temp[count] = a[i];
    }

    # pragma omp parallel num_threads(thread_count)
    {
        int thread_id = omp_get_thread_num();
        int chunksize = n / thread_count;
        int start = thread_id * chunksize;
        int end = (thread_id == thread_count - 1) ? n : start + chunksize;

        memcpy(&a[start], &temp[start], sizeof(int) * (end - start));
    }

    free(temp);
}