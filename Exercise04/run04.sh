#!/bin/bash
# Threads sweep (static), then schedule comparison at 24 threads
> results.csv
for t in 1 4 8 12 16 20 24; do
  OMP_NUM_THREADS=$t ./lab04d.o static | tee -a results_raw.txt | sed -E 's/.*threads=([0-9]+).*avg_time=([0-9.]+)/\1,\2/' >> results.csv
done
for s in static dynamic guided auto; do OMP_NUM_THREADS=24 ./lab04d.o $s; done | tee schedules.txt
