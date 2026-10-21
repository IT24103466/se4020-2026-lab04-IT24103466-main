#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define N 10000000L
static double term(long i) { double d = (double)i; return d - d * d + sqrt(2.0 * (d + 1.0)); }
int main(int argc, char **argv) {
    omp_set_num_threads(argc > 1 ? atoi(argv[1]) : 1);
    double t0, s = 0, c = 0, a = 0, r = 0;
    t0 = omp_get_wtime(); for (long i = 1; i <= N; i++) s += term(i); double ts = omp_get_wtime() - t0;
    t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 1; i <= N; i++) { double v = term(i);
        #pragma omp critical
        c += v; }
    double tc = omp_get_wtime() - t0;
    t0 = omp_get_wtime();
    #pragma omp parallel for
    for (long i = 1; i <= N; i++) { double v = term(i);
        #pragma omp atomic
        a += v; }
    double ta = omp_get_wtime() - t0;
    t0 = omp_get_wtime();
    #pragma omp parallel for reduction(+:r)
    for (long i = 1; i <= N; i++) r += term(i);
    double tr = omp_get_wtime() - t0;
    printf("threads=%d\nserial   S=%.6e t=%.4fs\ncritical S=%.6e t=%.4fs\natomic   S=%.6e t=%.4fs\nreduction S=%.6e t=%.4fs\n",
           omp_get_max_threads(), s, ts, c, tc, a, ta, r, tr);
    return 0;
}
