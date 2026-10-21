#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 10000000L
int main(int argc, char **argv) {
    int threads = argc > 1 ? atoi(argv[1]) : 1;
    double step = 1.0 / N, t0, serial, racy = 0, crit = 0, atom = 0;
    omp_set_num_threads(threads);

    t0 = omp_get_wtime(); serial = 0;
    for (long i = 1; i <= N; i++) { double x = (i - 0.5) * step; serial += 4.0 / (1.0 + x * x); }
    double ts = omp_get_wtime() - t0;

    t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 1; i <= N; i++) { double x = (i - 0.5) * step; racy += 4.0 / (1.0 + x * x); }
    double tr = omp_get_wtime() - t0;

    t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 1; i <= N; i++) { double x = (i - 0.5) * step;
        #pragma omp critical
        crit += 4.0 / (1.0 + x * x); }
    double tc = omp_get_wtime() - t0;

    t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 1; i <= N; i++) { double x = (i - 0.5) * step;
        #pragma omp atomic
        atom += 4.0 / (1.0 + x * x); }
    double ta = omp_get_wtime() - t0;

    printf("threads=%d\n", threads);
    printf("serial  pi=%.10f time=%.4fs\n", serial * step, ts);
    printf("racy    pi=%.10f time=%.4fs\n", racy * step, tr);
    printf("critical pi=%.10f time=%.4fs\n", crit * step, tc);
    printf("atomic  pi=%.10f time=%.4fs\n", atom * step, ta);
    return 0;
}
