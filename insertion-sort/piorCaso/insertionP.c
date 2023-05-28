#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void insertion_sort(int *v, unsigned int n) {
    unsigned int i, e;
    int aux;

    for (e = 1; e < n; e++) {
        i = e;

        while ((i > 0) && (v[i - 1] > v[i])) {
            aux = v[i - 1];
            v[i - 1] = v[i];
            v[i] = aux;

            i = i - 1;
                
        }
               

    }
}

int main(int argc, char **argv) {
    struct timespec a, b;
    unsigned int t, n;
    int i, *v;

    n = atoi(argv[1]);
    v = (int *) malloc(n * sizeof(int));
    srand(time(NULL));

    for (i = 0; i < n; i++) {
        v[i] = n - i;
    }
         
    clock_gettime(CLOCK_MONOTONIC, &b);
    insertion_sort(v, n);
    clock_gettime(CLOCK_MONOTONIC, &a);

    t = (a.tv_sec * 1e9 + a.tv_nsec) - (b.tv_sec * 1e9 + b.tv_nsec);

    printf("%u\n", t);

    free(v);

    return 0;
}