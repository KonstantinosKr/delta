# C files
C_SRC=motion.c input.c output.c loba.c contact.c tribal.c migration.c 

# ISPC files
ISPC_SRC= tribal.ispc bf.ispc

# ISPC targets
ISPC_TARGETS=avx2

# MPI
MPICC=mpicc
MPICXX=mpicc

# Zoltan paths
ZOLTANINC = -I/usr/local/include
ZOLTANLIB = -L/usr/local/lib -lzoltan

HULLINC = -Iext/hul
HULLLIB = -Lext/hul/ -lhul

# Program name
EXE=tribal

# Floating point type
REAL=double

# Debug version
DEBUG=no

# Do the rest
include common.mk
