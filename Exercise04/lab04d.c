#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define N 10000000L
/* usage: ./lab04d.o <schedule: static|dynamic|guided|auto> ; threads via OMP_NUM_THREADS */
int main(int argc, char **argv) {
    const char *kind = argc > 1 ? argv[1] : "static";
    omp_sched_t k = omp_sched_static;
    if (!strcmp(kind, "dynamic")) k = omp_sched_dynamic;
    else if (!strcmp(kind, "guided")) k = omp_sched_guided;
    else if (!strcmp(kind, "auto")) k = omp_sched_auto;
    omp_set_schedule(k, 0);
    const double step = 1.0 / N; double times[10], pi = 0;
    for (int run = 0; run < 10; run++) {
        double sum = 0.0, x; double t0 = omp_get_wtime();
        #pragma omp parallel for schedule(runtime) private(x) firstprivate(step) shared(sum)
        for (long i = 1; i <= N; i++) { x = (i - 0.5) * step;
            double v = 4.0 / (1.0 + x * x);
            #pragma omp atomic
            sum += v; }
        times[run] = omp_get_wtime() - t0; pi = sum * step;
    }
    double avg = 0; for (int i = 3; i < 10; i++) avg += times[i]; avg /= 7;
    printf("threads=%d schedule=%s pi=%.10f avg_time=%.4f\n", omp_get_max_threads(), kind, pi, avg);
    return 0;
}
