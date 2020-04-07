#!/bin/bash
# Stream V. 5.10
## Prepare build on Supermuc ##

module load intel/xe_2017.2

# Build
echo "build gcc-noomp"
gcc -O3           -U_OPENMP stream.c -o stream-gcc-noomp
echo "done!"
echo "build icc-noomp"
icc -O3 -xHost    -U_OPENMP stream.c -o stream-icc-noomp
echo "done!"
echo "build gcc-omp"
gcc -O3 -fopenmp            stream.c -o stream-gcc-omp
echo "done!"
echo "build icc-omp"
icc -O3 -xHost -qopenmp     stream.c -o stream-icc-omp
echo "done!"
