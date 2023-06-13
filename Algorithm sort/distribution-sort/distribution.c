#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int max(int *v, int n) {
    int max = v[0];

    for (int i = 1; i < n; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }

    return max;
}

int min(int *v, int n) {
    int min = v[0];

    for (int i = 1; i < n; i++) {
        if (v[i] < min) {
            min = v[i];  
        }
    }

    return min;
}

void distribution_sort(int *v, unsigned int n) {
    int s = min(v, n);
    int b = max(v, n);
    int d;

    int *c = (int *) malloc((b - s + 2) * sizeof(int));
    int *w = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < (b - s + 2); i++) {
        c[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        c[v[i] - s + 1] = c[v[i] - s + 1] + 1;
    }

    for (int i = 1; i < (b - s + 2); i++) {
        c[i] = c[i] + c[i - 1];
    }

    for (int i = 0; i < n; i++) {
        d = v[i] - s + 1;
        w[c[d] - 1] = v[i];
        c[d] = c[d] - 1;
    }

    for (int i = 0; i < n; i++) {
        v[i] = w[i];
    }

    free(c);
    free(w);
}

int main(int argc, char **argv) {
    struct timespec a, b;
    unsigned int t, n;
    int i, *v;

    n = atoi(argv[1]);
    v = (int *) malloc(n * sizeof(int));
    srand(time(NULL));

    for (i = 0; i < n; i++) {
        v[i] = rand() % 1000;
        
    }
       
    clock_gettime(CLOCK_MONOTONIC, &b);
    distribution_sort(v, n);
    clock_gettime(CLOCK_MONOTONIC, &a);

    t = (a.tv_sec * 1e9 + a.tv_nsec) - (b.tv_sec * 1e9 + b.tv_nsec);

    printf("%u\n", t);   

    free(v);

    return 0;
}

