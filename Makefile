# C files
C_SRC= 
CPP_SRC = input.cpp output.cpp delta.cpp migration.cpp loba.cpp contact.cpp forces.cpp dynamics.cpp tasksys.cpp

# ISPC files
ISPC_SRC= delta.ispc bf.ispc

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
EXE=delta

# Floating point type
REAL=double

# Debug version
DEBUG=yes

# Do the rest
include common.mk
