# C files
C_SRC= 
CPP_SRC = motion.cpp input.cpp output.cpp loba.cpp contact.cpp vega.cpp migration.cpp tasksys.cpp

# ISPC files
ISPC_SRC= vega.ispc bf.ispc parmes.ispc partition.ispc condet.ispc forces.ispc dynamics.ispc shapes.ispc obstacles.ispc

# ISPC targets
ISPC_TARGETS=sse2,sse4,avx

# MPI
MPICC=mpicc
MPICXX=mpic++

CC=gcc
CXX=g++

# Zoltan paths
ZOLTANINC = -I/usr/local/include
ZOLTANLIB = -L/usr/local/lib -lzoltan

HULLINC = -Iext/hul
HULLLIB = -Lext/hul -lhul

# Python paths
PYTHONINC=-I/usr/include/python2.7
PYTHONLIB=-L/usr/lib -lpython2.7

# Program name
EXE=vega

# Floating point type
REAL=double

# Debug version
DEBUG=no

# Do the rest
include common.mk
