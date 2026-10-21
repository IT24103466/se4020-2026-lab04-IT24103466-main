#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 10000000L
int main(int argc, char **argv) {
    omp_set_num_threads(argc > 1 ? atoi(argv[1]) : 1);
    long hs = 0, hc = 0, ha = 0; double t0;
    unsigned int seed = 42;
    t0 = omp_get_wtime();
    for (long i = 0; i < N; i++) { double x = rand_r(&seed) / (double)RAND_MAX, y = rand_r(&seed) / (double)RAND_MAX; if (x*x + y*y <= 1.0) hs++; }
    double ts = omp_get_wtime() - t0;
    t0 = omp_get_wtime();
    #pragma omp parallel
    { unsigned int sd = 42 + omp_get_thread_num();
      #pragma omp for
      for (long i = 0; i < N; i++) { double x = rand_r(&sd) / (double)RAND_MAX, y = rand_r(&sd) / (double)RAND_MAX;
        if (x*x + y*y <= 1.0) {
          #pragma omp critical
          hc++; } } }
    double tc = omp_get_wtime() - t0;
    t0 = omp_get_wtime();
    #pragma omp parallel
    { unsigned int sd = 42 + omp_get_thread_num();
      #pragma omp for
      for (long i = 0; i < N; i++) { double x = rand_r(&sd) / (double)RAND_MAX, y = rand_r(&sd) / (double)RAND_MAX;
        if (x*x + y*y <= 1.0) {
          #pragma omp atomic
          ha++; } } }
    double ta = omp_get_wtime() - t0;
    printf("threads=%d\nserial   pi=%.6f t=%.4fs\ncritical pi=%.6f t=%.4fs\natomic   pi=%.6f t=%.4fs\n",
           omp_get_max_threads(), 4.0*hs/N, ts, 4.0*hc/N, tc, 4.0*ha/N, ta);
    return 0;
}
