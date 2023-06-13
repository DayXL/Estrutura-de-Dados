#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *v, unsigned int s, unsigned int m, unsigned int e) {

    unsigned int p = s;
    unsigned int q = m + 1;

    int *w = (int *) malloc((e - s + 1) * sizeof(int));

    for (int i = 0; i < (e - s + 1); i++) {
        if ((q > e) || ((p <= m) && v[p] < v[q])) {
            w[i] = v[p];
            p = p + 1;

        }

        else {
            w[i] = v[q];
            q = q + 1;

        }        

    }

    for (int i = 0; i < (e - s + 1); i++) {
        v[s + i] = w[i];    

    }
    
    free(w);

}

void merge_sort(int *v, unsigned int s, unsigned int e) {
    unsigned int m;
    
    if (s < e) {
        
        m = ((s + e) / 2);

        merge_sort(v, s, m);
        merge_sort(v, m + 1, e);
        merge(v, s, m, e);

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
        v[i] = rand() % 1000;
        
    }
       
    clock_gettime(CLOCK_MONOTONIC, &b);
    merge_sort(v, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &a);

    t = (a.tv_sec * 1e9 + a.tv_nsec) - (b.tv_sec * 1e9 + b.tv_nsec);

    printf("%u\n", t);   

    free(v);

    return 0;
}



