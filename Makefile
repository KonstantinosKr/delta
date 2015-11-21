# C files
C_SRC=motion.c input.c output.c loba.c contact.c vega.c migration.c 

# ISPC files
ISPC_SRC= vega.ispc bf.ispc

# ISPC targets
ISPC_TARGETS=avx2

# MPI
MPICC=mpicc
MPICXX=mpicc

# Zoltan paths
ZOLTANINC = -I/usr/local/include
ZOLTANLIB = -L/usr/local/lib -lzoltan

HULLINC = -Iext/hul
HULLLIB = -Lext/hul -lhul

# Program name
EXE=vega

# Floating point type
REAL=double

# Debug version
DEBUG=no

# Do the rest
include common.mk
