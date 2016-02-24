# C files
C_SRC= 
CPP_SRC = input.cpp geom.cpp material.cpp output.cpp log.cpp delta.cpp migration.cpp loba.cpp contact.cpp bf.cpp forces.cpp dynamics.cpp hybrid.cpp 

# ISPC files
ISPC_SRC=

# ISPC targets
ISPC_TARGETS=avx

# MPI
MPICC=mpicc
MPICXX=mpicc
#MPICC = mpicc
#MPICXX = mpiicpcp

CC=gcc
CXX=g++

# Zoltan paths
ZOLTANINC = -I/usr/local/include
ZOLTANLIB = -L/usr/local/lib -lzoltan
#ZOLTANINC = -I/home/hudson/pg/rfmw74/zlocal/include
#ZOLTANLIB = -L/home/hudson/pg/rfmw74/zlocal/lib -lzoltan

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
