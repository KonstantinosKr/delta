# Stream V. 5.10


## Prepare build on Hamilton ##

module load intel/xe_2017.2



# Build

gcc -O3           -U_OPENMP stream.c -o stream-gcc-noomp
icc -O3 -xHost    -U_OPENMP stream.c -o stream-icc-noomp
gcc -O3 -fopenmp            stream.c -o stream-gcc-omp
icc -O3 -xHost -qopenmp     stream.c -o stream-icc-omp

