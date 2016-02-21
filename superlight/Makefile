# C files
C_SRC= 
CPP_SRC = input.cpp geom.cpp output.cpp delta.cpp bf.cpp contact.cpp forces.cpp dynamics.cpp

# ISPC files
ISPC_SRC= 

# ISPC targets
ISPC_TARGETS=avx

CC=gcc
CXX=g++

HULLINC = -Iext/hul
HULLLIB = -Lext/hul -lhul

# Python paths
#PYTHONINC=-I/usr/include/python2.7
#PYTHONLIB=-L/usr/lib -lpython2.7
PYTHONINC=-I/Library/Frameworks/Python.framework/Versions/3.4/include/python3.4mi
PYTHONLIB=-L/Library/Frameworks/Python.framework/Versions/3.4/lib/ -lpython3.4

# Program name
EXE=delta

# Floating point type
REAL=double

# Debug version
DEBUG=yes

# Do the rest
include common.mk
