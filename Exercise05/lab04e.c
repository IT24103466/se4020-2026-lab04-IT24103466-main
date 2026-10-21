#include <stdio.h>
#include <omp.h>
#define N 100000
int main(void) {
    long long sum; long long cntA = 0, cntBinit = 0, cntBloop = 0, cntBadd = 0; double t0;
    /* A */
    sum = 0; t0 = omp_get_wtime();
    #pragma omp parallel for
    for (int r = 1; r <= N; r++) {
        #pragma omp atomic
        sum = sum + r;
        #pragma omp atomic
        cntA++;
    }
    printf("A: sum=%lld line runs=%lld time=%.6fs\n", sum, cntA, omp_get_wtime() - t0);
    /* B */
    sum = 0; t0 = omp_get_wtime();
    #pragma omp parallel
    {
        long long local_sum = 0;
        #pragma omp atomic
        cntBinit++;
        #pragma omp for
        for (int r = 1; r <= N; r++) {
            local_sum += r;
            #pragma omp atomic
            cntBloop++;
        }
        #pragma omp atomic
        sum += local_sum;
        #pragma omp atomic
        cntBadd++;
    }
    printf("B: sum=%lld local_sum init runs=%lld loop body runs=%lld atomic add runs=%lld time=%.6fs\n", sum, cntBinit, cntBloop, cntBadd, omp_get_wtime() - t0);
    /* C */
    sum = 0; t0 = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (int r = 1; r <= N; r++) { sum += r; }
    printf("C: sum=%lld time=%.6fs\n", sum, omp_get_wtime() - t0);
    return 0;
}
